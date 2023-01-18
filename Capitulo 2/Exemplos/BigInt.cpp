#include "BigInt.h"
#include <iostream>
#include <cstring>
#include <assert.h>
#include <iomanip>

// << Definição de constantes >>
static const size_t BITS_WORD = sizeof(word)*8;
// Base de numeração (65536)
static const dword B_Radix = 0xFFFFL+1;
// Maior algarismo na base B_Radix == (B_Radix - 1)
static const word MAX_DIGIT = 0xFFFF;

#define oper(op) \
    BigTmp operator op (const BigInt &a, const BigInt &b) \
        { return BigTmp(max(a.size(), b.size())+1, a) op b; }
    oper(+) oper(-)
#undef oper

// Construtores
BigInt::BigRep::BigRep(const char *s, StrType typ, size_t dim){
    switch (typ)
    {
    case TEXT: text2big(s, strlen(s), dim); break;
    case HEX: hex2big(s, strlen(s), dim); break;
    case DEC: default: dec2big(s, strlen(s), dim); break;
    }
    count = 0;
}
BigInt::BigRep::BigRep(long n, size_t dim) {
    count = 0;              // Iniciar contador de referências
    if (n>=0)
        signal = 1;
    else {
        n = -n; signal = -1;
    }
    sz = (dword(n) > MAX_DIGIT) ? 2 : 1;
    v = allocate(dim);
    v[0] = word(n % B_Radix);
    if (sz == 2)
        v[1] = word(n / B_Radix);
}
BigInt::BigRep::BigRep(const BigRep &b, size_t dim){
    count = 0; signal = b.signal; sz = b.sz;
v = allocate(sz, dim);                  // Reservar espaço.
    memcpy(v, b.v, sz * sizeof(word));  // Copiar.
}
// Sobrecarga dos operados de afetação
BigInt & BigInt::operator=(long n) {
    if (brp->count > 1) brp = new BigRep(n);
    else *brp = n;
    return *this;
}
void BigInt::BigRep::operator= (long n) {
    if (n >= 0) signal = 1;
    else{ n = -n; signal = -1; }
    sz = (dword(n) > MAX_DIGIT ? 2 : 1);    // DIM_MIN >= 2
    v[0] = word (n%B_Radix);
    if (sz == 2)
        v[1] = word(n/B_Radix);
}                                // valor DIM_MIN por omissão
word * BigInt::BigRep::allocate(size_t dimension) {
    for (dim = DIM_MIN; dim < dimension; dim *= 2);
    return new word[dim];
}
// Método Estatico que compara objectos BigInt em valor
int BigInt::BigRep::cmp(const BigRep &a, const BigRep &b) {
    if (a.signal != b.signal) return a.signal;
    return (a.signal == -1) ? cmpMod(b,a) : cmpMod(a,b);
}
void BigInt::BigRep::reserve(size_t newDimension) {
    if (dim < newDimension) {
        word *paux=allocate(newDimension);
        memcpy(paux, v, sz * sizeof(word));
        delete [] v;
        v=paux;
    }
}
// Método estatico que compara em modulo objectos BigRep
int BigInt::BigRep::cmpMod(const BigRep &a, const BigRep &b) {
    if (a.sz != b.sz) return a.sz > b.sz ? 1:-1;
    size_t index = a.sz -1;
    for (;a.v[index]==b.v[index]; --index)
        if(index == 0) return 0;
    return (a.v[index]>b.v[index]) ? 1 : -1;
}
// Sobrecarga de operadores aritméticos
void BigInt::BigRep::operator+=(const BigRep &b){
    int cmp = cmpMod(*this, b);
    // Normalizar para o addNorm() ou para o subNorm()
    const BigRep &max = cmp > 0 ? *this : b;    // Operando Esquerdo
    const BigRep &min = cmp > 0 ? b : *this;    // Operando Direito
    if (signal == b.signal) addNorm(*this, max, min);
    else if (cmp != 0){
        subNorm(*this, max, min); signal = max.signal;
    } else { signal = 1; sz = 1; v[0]=0; }      //*this = 0
}
void BigInt::BigRep::operator-=(const BigRep &b){
    int cmp = cmpMod(*this, b);
    // Normalizar para o addNorm() ou para o subNorm()
    const BigRep &max = cmp > 0 ? *this : b;    // Operando Esquerdo
    const BigRep &min = cmp > 0 ? b : *this;    // Operando Direito
    if (signal != b.signal) addNorm(*this, max, min);
    else if (cmp != 0) {
        subNorm(*this, max, min); 
        if (cmp<0) 
            signal = -signal;
    }else {signal = 1; sz = 1; v[0] = 0; }      // *this = 0
}
BigInt::BigRep * BigInt::BigRep::operator*(const BigRep &b) const{
    if (isZero() || b.isZero()) return new BigRep(0L);
    BigRep &aux = *new BigRep(0L, sz + b.sz + 1);
    if(cmpMod(*this, b) >= 0)       // Normalizar para a multNorm()
        multNorm(aux, *this, b);    // Operando esquerdo é *this
    else multNorm(aux, b, *this);   // Operando esquerdo é b
    aux.signal = signal*b.signal;
    return &aux;
}
BigInt::BigRep * BigInt::BigRep::operator/(const BigRep &d) const{
    assert(!d.isZero());    // Divisão por zero
    if (isZero()) return new BigRep(0L);
    int n = cmpMod (*this, d);
    if (n==0) return new BigRep(1L);
    if (n < 0) return new BigRep(0L);
    BigRep *quo = new BigRep(0L, sz-d.sz + 1);
    if (d.sz==1) { word rest; divNorm(*quo, rest,*this,d.v[0]); }
    else { BigRep rest(0L,d.sz+1); divNorm(*quo,rest,*this, d); }
    quo->signal = (signal == d.signal) ? 1: -1;
    return quo;
}
BigInt::BigRep * BigInt::BigRep::operator%(const BigRep &d) const {
    if (d.isZero()) return new BigRep(*this); // Divisão por zero.
    int n = cmpMod(*this, d);
    if (n == 0) return new BigRep(0L);
    if (n < 0) return new BigRep(*this);
    if (d.sz == 1) {
        BigRep &aux = *new BigRep(0L, sz);
        word rest; divNorm(aux, rest, *this, d.v[0]);
        aux.v[0] = rest; aux.sz = 1;
        return &aux;
    }
    BigRep &rest = *new BigRep(0L, d.sz+1);
    BigRep quo(0L, sz - d.sz + 1);
    divNorm(quo, rest, *this, d);
    rest.signal = signal;
    return &rest;
}
// << Operações aritméticas >>
void BigInt::BigRep::addNorm(BigRep &add, const BigRep &max, const BigRep &min) {
    add.reserve(max.sz +1);
    size_t i;           // indice para percorrer os algarismos
    word carry=0;       // Arrasto.
    dword buff;         // Auxiliar para as somas intermédias
    word *pAdd = add.v, *pMax = max.v, *pMin = min.v;
    // Somar algarismos homologos (existem 2 operandos)
    for (i=0; i < min.sz; ++i) {
        buff = dword (pMax[i]) + pMin[i] + carry;
        if (buff >= B_Radix) { buff -= B_Radix; carry = 1; }
        else carry = 0;
        pAdd[i] = word(buff);
    }
    // Somar ao maior dos operandos o arrasto
    for (; i < max.sz; ++i) {
        buff = dword(pMax[i]) + carry;
        if( buff >= B_Radix) { buff -= B_Radix; carry = 1; }
        else carry = 0;
        pAdd[i] = word(buff);
    }
    // Atualizar o sz e acrescentar ao BigInt add o arrasto
    add.sz = max.sz; if (carry) pAdd[add.sz++] = 1;
}
void BigInt::BigRep::subNorm(BigRep &sub, const BigRep &max, const BigRep &min) {
    sub.reserve(max.sz); dword borrow=0; // Arrasto
    word *pSub=sub.v, *pMax=max.v, *pMin=min.v; size_t i;
    // Subtrair os algarismos homologos (existem 2 operandos)
    for (i=0; i < min.sz; ++i){
        if (pMax[i] >= (pMin[i] + borrow)){    
            pSub[i] = word(pMax[i]-(pMin[i]+borrow)); borrow = 0;
        } else {
            pSub[i] = word ((B_Radix+pMax[i])-(pMin[i]+borrow));
            borrow = 1;
        }
    }
    // Subtrair ao maior dos operandos o arrasto
    for(; i<max.sz; ++i) {
        if (pMax[i] < borrow) {
            pSub[i]=word(pMax[i] + B_Radix - borrow); borrow = 1;
        } else { pSub[i] = word(pMax[i]-borrow); borrow = 0; }
    }
    // Atualizar o sz, retirando os zeros à esquerda
    sub.sz = max.sz;
    while (sub.sz > 1 && pSub[sub.sz-1]==0) --sub.sz;
}
void BigInt::BigRep::shiftLeft(word nShifts){
    word shift_bits = nShifts % BITS_WORD,
         shift_words = nShifts / BITS_WORD;
    if (shift_words) {      // Deslocamento em bloco
        reserve(sz + shift_words + (shift_bits != 0));
        memmove(v+shift_words, v, sz * sizeof(word));
        memset(v, 0, shift_words * sizeof(word));
        sz += shift_words;
    }
    if (shift_bits) {
        word *p = v, carry = 0; dword daux;
        for (size_t i = sz; i != 0; --i) {
            daux = dword(*p) << shift_bits;
            *(p++) = word(daux%B_Radix) | carry;
            carry = word(daux / B_Radix);
        }
        if (carry != 0) { reserve (sz+1); v[sz++] = carry; }
    }
}
void BigInt::BigRep::shiftRight(word nShifts){
    word shift_bits = nShifts % BITS_WORD,
         shift_words = nShifts / BITS_WORD;
    if (shift_words) {  // Deslocamento em bloco
        sz -= shift_words;
        memmove(v, &v[shift_words], sz * sizeof(word));
    }
    if (shift_bits) {
        word *p = v, carry = 0; dword daux;
        shift_bits = BITS_WORD - shift_bits;
        for (size_t i = sz; i != 0; --i) {
            daux = dword (*(p + i -1)) << shift_bits;
            *(p + i - 1) = word ((daux/B_Radix) | carry);
            carry = word(daux % B_Radix);
        }
        if (sz != 0 && v[sz-1] == 0) --sz;
    }
    if (sz == 0) { sz = 1; signal = 1; }
}
void BigInt::BigRep::multNorm(BigRep &prod, const BigRep &br, word n) {
    assert(br.sz >= 1);
    dword daux, carry = 0;
    for (size_t i = 0; i < br.sz; ++i) {
        daux = carry + dword(br.v[i]) * n;
        carry = daux / B_Radix;
        prod.v[i] = word(daux % B_Radix);
    }
    prod.sz = br.sz;
    if (carry) {
        prod.reserve(prod.sz+1); prod.v[prod.sz++] = word(carry);
    }
}
void BigInt::BigRep::multNorm(BigRep &prod, const BigRep &max, const BigRep &min) {
    assert(max.sz >= min.sz);
    dword carry = 0;
    BigRep brAux(0L, max.sz + min.sz + 1);
    for (size_t i = 0; i < min.sz; ++i) {
        if ( min.v[i] ) {
            multNorm(brAux, max, min.v[i]);
            brAux <<= (i * BITS_WORD);
            addNorm(prod, brAux, prod);
        }
    }
}
void BigInt::BigRep::divNorm(BigRep &quo, word &rest, const BigRep &dd, word n) {
    dword aux, carry=0;
    for(size_t i = dd.sz; i!=0; ) {
        aux = dd.v[--i] + carry * B_Radix;
        if (aux < dword(n)) { carry = aux; quo.v[i] = 0; }
        else { carry = aux % n; quo.v[i] = word(aux / n); }
    }
    if ((quo.sz = dd.sz) > 1 && quo.v[quo.sz-1] == 0) --quo.sz;
    rest = word(carry);
}
void BigInt::BigRep::divNorm(BigRep &quo, BigRep &rest, const BigRep &cdd, const BigRep &cd) {
    assert (cmpMod(cdd,cd) >= 0);
    BigRep dd(cdd, cdd.sz +1), d(cd); d.signal = 1;
    int nShifts = 0;    // Número de shifts da normalização.
    word gess_q;        // Algarismo estimado da divisão
    // Auxiliar para a multiplicação do divisor por gess_q
    BigRep buf(0L, d.sz +1);
    // Normalizar dividendo (dd) e divisor (d) por nShifts
    while (d.v[d.sz-1] < B_Radix/2)
        {dd<<=1; d<<=1; nShifts++; }
    // Carregar resto parcial com os algarismos de maior peso
    // do dividendo
    rest.v[0] = dd.v[--dd.sz];
    while (cmpMod(rest, d) < 0)
        {rest <<= 16 ; rest.v[0] = dd.v[--dd.sz];}
    while (cmpMod(rest, d) >= 0) {
        // Estimar o algarismo a acrescentar ao quociente
        gess_q = gessDig(rest, d);
        // Subtrair ao resto o produto do algarismo estimado
        // pelo divisor.
        multNorm(buf, d, gess_q);
        rest -= buf;
        // Enquanto o algarismo estimado pecar por excesso,
        // decrementa de um e corrige o resto
        while( rest.signal == -1) { gess_q--; rest += d; }
        // Acrescentar ao quociente o algarismo estimado
        quo.v[0] = gess_q;
        // Carregar mais um algarismo do dividendo no resto
        // parcial e acrescentar um zero ao quociente
        while (rest < d && dd.sz > 0) {
            quo <<= BITS_WORD; rest <<= BITS_WORD;
            rest.v[0] = dd.v[--dd.sz];
        }
    }
    // Corrigir o resto dos shifts da normalização.
    rest >>= nShifts;
}
// Estimativa os algarismo do quociente
word BigInt::BigRep::gessDig(BigRep &aux, BigRep &d) {
    dword lead_aux = aux.v[aux.sz-1];
    if (aux.sz > d.sz) {
        lead_aux *= B_Radix; lead_aux += aux.v[aux.sz-2];
    }
    dword dig = lead_aux / d.v[d.sz-1];
    if (dig >= B_Radix) return word(B_Radix-1);
    return word(dig);
}
void BigInt::BigRep::text2big(const char *s, size_t len, size_t dim) {
    // Calculo da dimensaão minima do array de words
    sz = len /sizeof(word) + (len % sizeof(word) != 0);
    // Reserva espaço(maior que sz e menor ou igual a dim)
    v = allocate(sz, dim);
    v [sz-1] = 0;       // Garantir zero nos bits não usados
    signal = 1;         // Positivo
    memcpy(v, s, len);   // Copia os dados
}
void BigInt::BigRep::hex2big(const char *s, size_t len, size_t dim) {
    const char * saux = s;
    const size_t BITS_PER_DIGIT = 4;
    const size_t DIGITS_PER_WORD = BITS_WORD/BITS_PER_DIGIT;
    if (*saux == '-'){
        signal = -1;
        ++saux;
    } else {
        signal = 1;
    }
    while (*saux == '0') ++saux;    // Remoção dos zeros à esquerda
    len -= saux - s;                // Atualização do comprimento
    // Calculo da dimensao minima do array de words
    sz = len / DIGITS_PER_WORD + (len % DIGITS_PER_WORD != 0);
    v = allocate(sz, dim);          // Reserva de espaço
    v[sz-1] = 0;                    // Garante zero nos bits não usados
    // Número de digitos a converter para word
    size_t nOfDigs = DIGITS_PER_WORD;
    for (size_t index = 0; len; ++index, len-=nOfDigs) {
        if (len < DIGITS_PER_WORD)  // Ultimo conjunto a converter
            nOfDigs=len;
        // Converter digs caracteres em word
        word aux = 0;
        for (size_t j = nOfDigs; j; --j) {
            aux <<= BITS_PER_DIGIT;
            aux += tolower(saux[len-j]) - (isdigit(saux[len-j])? '0' : 'a' - 10);
        }
        v[index] = aux;
    }
}
void BigInt::BigRep::dec2big(const char * s, size_t len, size_t dim) {
    int aux_signal;
    if(*s == '-') {
        aux_signal = -1;
        ++s;
    } else {
        aux_signal = 1; 
    }
    while (*s == '0' ) ++s;         // Remover zeros à esquerda
    v = allocate(len/4, dim);       // Número de words é len/4
    sz = 1; v[0] = 0; signal = 1;   // Colocar *this = 0;
    while (*s) {
        *this *= 10;
        *this += *s++ -'0';
    }
    signal = aux_signal;            // Atualizar o sinal
}
void BigInt::BigRep::operator++() {
    size_t i = 0;
    while (i < sz && v[i] == MAX_DIGIT)
        v[i++]=0;
    if (i==sz) {
        reserve(++sz);
        v[i] = 1;
    }else
        ++v[i];
}
void BigInt::BigRep::big2txt(std::ostream &os) const {
    for (size_t i = 0; i < sz * 2; ++i)
        os << ( ( char * ) v )[i];
}

void BigInt::BigRep::big2hex(std::ostream &os) const {
    if(signal == -1) os << '-';
    char c = os.fill(); // Obter o caracter de fill
    for (word * aux = v + sz; aux != v; )
        os << std::setw(4) << std::setfill('0') << *(--aux) << ' ';
    os << std::setfill(c);  // Repor estado inicial.  
}
void BigInt::BigRep::operator+=(word b){
	if (signal == 1) 
		addNorm(*this, *this, b);
	else
		if (cmpMod(*this, b) > 0) 
			subNorm(*this, *this, b);
		else {
			v[0] = b - v[0] ;
			signal = 1;
		}
}

void BigInt::BigRep::big2dec(BigRep &bb, std::ostream &os) {
    if (bb.isZero()) return;
    word rest;
    divNorm(bb, rest, bb, 10);
    big2dec(bb, os);
    os << std::dec << rest;
}
void BigInt::BigRep::big2dec(std::ostream &os) const {
    if(isZero()) { os << '0'; return; }
    if (signal == -1) os << '-';
    BigRep aux(*this); big2dec(aux, os);
}
BigInt& BigInt::operator+=(const long n){
	long b = (n<0) ? -n : n;
	if (b > MAX_DIGIT) *this += BigInt(n);
	else {
		unlink();
		if (n < 0) *brp -= word(b);
		else *brp += word (b);
	}
	return *this;
}

//<<Operacoes de subtraccao com long >>
void BigInt::BigRep::subNorm(BigRep& sub, const  BigRep& br, word n) {
	assert(sub.dim >= br.sz);
	word *pSub=sub.v;
	const word *pbr = br.v;
	dword borrow = n;
	for (size_t i=0; i < br.sz; ++i){
		if (pbr[i] < borrow) { pSub[i]=word(pbr[i] + B_Radix - borrow); borrow=1; } 
		else { pSub[i] = word(pbr[i]-borrow); borrow=0;}
	}
	assert (!borrow);
	sub.sz = br.sz;
	while(sub.sz > 1 && pSub[sub.sz -1 ] == 0) --sub.sz;
}

void BigInt::BigRep::operator-=(word b){
	if (signal == -1) 
		addNorm(*this, *this, b);
	else {
		int cmp = cmpMod(*this, b);
		if (cmp == 0) {
			v[0] = 0; signal = 1;
		}
		else if (cmp > 0) 
			subNorm(*this, *this, b);
		else {
			v[0] = b - v[0] ;
			signal = - 1;
		}
	}
}

BigInt& BigInt::operator-=(const long n){
	long b = (n < 0) ? -n : n;
	if (b > MAX_DIGIT) *this -= BigInt(n);
	else {
		unlink();
		if (n < 0) *brp += word(b);
		else *brp -= word (b);
	}
	return *this;
}

// <<Operacoes de multiplicao com long>>
void BigInt::BigRep::operator*=(word n){
	if (isZero()) return;
	if (n == 0) { sz = 1; v[0] = 0; signal = 1;  }
	if ( n != 1 ) 
		multNorm(*this, *this, n);
}

// <<Operacoes divisao com long>>
void BigInt::BigRep::operator/=(word n) {
	assert(n);
	word rest;
	divNorm(*this, rest, *this, n);
	if (isZero()) signal = 1;
}

void BigInt::BigRep::operator %=(word n) {
	 if (n == 0) return;
	 word rest;
	 divNorm(*this, rest, *this, n); v[0] = rest; 
	 sz = 1; 
}

#define oper(op) \
	BigInt& BigInt::operator op (const long n) { \
		if (n == 1) return *this; \
		long b = ( n > 0) ? n : -n; \
		if (b > MAX_DIGIT) return *this op BigInt(n); \
		unlink(); \
		*brp op word(b); \
		if (n < 0) brp->neg();\
		return *this; \
	}
    oper(*=)   oper(/=) 
#undef oper


std::ostream &txt(std::ostream &o){
    o.unsetf(std::ios_base::dec|std::ios_base::hex);
    return o;
}

std::ostream &operator<<(std::ostream &os, const BigInt &bi) {
    // Caso a flag ios_base::dec esteja activa devido
    // à prévia inserção no ostream do manipulador dec
    if (os.flags() & std::ios_base::dec)
        bi.big2dec(os);
    // Caso a flag ios_base::hex esteja activa devido
    // à prévia inserção no ostream do manipulador hex
    else if (os.flags() & std::ios_base::hex) bi.big2hex(os);
    // Caso amabas as flag anteriores não estejam activadas
    // devido à prévia inserção no ostream do manipulador txt
    else bi.big2txt(os);
    return os;
}

/*int main(int argc, char ** argv){
    BigInt n1 = 99999999L, n2, n3=6871209L, n4=724695L, n5;
    std::cout << n1 << '-' << n3 << '-' << n4 << '-' << n5 << std::endl;
    clock_t tempoi = clock();
    for (int i=0;i<10; ++i) {
        n3 = n1 * n3;
        n3 += BigInt("-123456780", BigInt::DEC);
        n5 = n3/n4;
        n5 = n5 * n4 + n3 % n4 - n3;
        n3 -= -123456780L;
        if (n5 != 0L) std::cout << "ERRO" << std::endl;
    }
    clock_t tempof = clock();
    std::cout << "\nn3=" << n3 << "\nn5=" << n5 << std::endl;
    std::cout << "Tempo = " << float(tempof-tempoi)/CLOCKS_PER_SEC << "secs" << std::endl;
    BigInt bHex("f7cd4", BigInt::HEX);
	BigInt bDec("765432", BigInt::DEC);
	BigInt bTxt ("Texto a cifrar", BigInt::TEXT);
	std::cout << std::hex << bHex << std::endl; // Valor em hexadecimal.
	std::cout << std::dec << bHex << std::endl; // Valor em decimal.
	std::cout << std::dec << bDec << std::endl; // Valor em decimal.
	std::cout << std::hex << bDec << std::endl; // Valor em hexadecimal.
	std::cout << txt << bTxt << std::endl; // Texto.
	std::cout << std::hex << bTxt << std::endl; // Valor em hexadecimal.
    return 0;
}*/