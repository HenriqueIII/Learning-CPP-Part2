#include <iostream>
#include <cstring>

// << Definição de tipos >>
typedef unsigned char byte;     // sizeof == 8 bits == 255
typedef unsigned short word;    // sizeof == 16 bits == 65535
typedef unsigned long dword;    // sizeof == 32 bits == 4 294 967 295

// << Definição de constantes >>
static const size_t BITS_WORD = sizeof(word)*8;
// Base de numeração (65536)
static const dword B_Radix = 0xFFFFL+1;
// Maior algarismo na base B_Radix == (B_Radix - 1)
static const word MAX_DIGIT = 0xFFFF;

class BigTmp;

class BigInt {
public:
// enumerado de tipos de strings para construtor
    enum StrType{TEXT, HEX, DEC};
private:
    // Capacidade minima do vector representação
    static const byte DIM_MIN = 2;      // Nunca inferior a 2;
// << Classes Internas >>
    class BigRep{       // Representação(body)
        friend class BigInt;
        int count;      // BigInts que partilham este BigRep
        size_t sz;      // Número de digitos
        size_t dim;     // Dimensão do espaço alojado
        int signal;     // -1 se negativo, 1 se positivo
        word * v;       // A representação
    // << Metodos Auxiliares >>
        // Reserva de Memória
        word * allocate(size_t dimension);
        word * allocate(size_t s, size_t d) {return allocate(d < s ? s : d); }
        void reserve(size_t newDimension);
        // Auxiliar para escrita em decimal num ostream
        static void big2dec(BigRep &bb, std::ostream &os);
        // Método de comparação
        static int cmp (const BigRep &a, const BigRep &b);
        // Métodos de comparação em módulo
        static int cmpMod(const BigRep &a, const BigRep &b);
        static int cmpMod(const BigRep &a, word b);
        // Métodos executores das operações shift
        void shiftLeft(const word nShifts);
        void shiftRight(const word nShifts);
        // Métodos executores das operações normalizadas
        static void addNorm(BigRep &add, const BigRep &max, const BigRep &min);
        static void addNorm(BigRep &add, const BigRep &br, word);
        static void subNorm(BigRep &sub, const BigRep &max, const BigRep &min);
        static void subNorm(BigRep &sub, const BigRep &max, word);
        static void multNorm(BigRep &prod, const BigRep &max, const BigRep &min);
        static void multNorm(BigRep &prod, const BigRep &br, word);
        // Método auxiliar para a divisão normalizada
        static word gessDig (BigRep &aux, BigRep &d);
        static void divNorm(BigRep &quo, BigRep &rest, const BigRep &dd, const BigRep &d);
        static void divNorm(BigRep &quo, word &rest, const BigRep &&dd, word);
    public:
    // << Construtores e destrutor >>
        BigRep(const char *s, StrType typ, size_t dim = DIM_MIN);
        BigRep(long = 0, size_t = DIM_MIN);
        BigRep(const BigRep &, size_t = DIM_MIN);
        ~BigRep() { delete [] v; }
    // << Sobrecarga dos operadores de afetação >>
        void operator=(long n);
        void operator= (const BigRep &);
    // << Métodos de Acesso >>
        bool isOdd() const  { return (v[0]&1);  }
        bool isEven() const { return !(v[0]&1); }
        bool isZero() const { return (sz==1 && v[0]==0); }
        bool isOne() const  { return sz==1 && v[0]==1 && signal==1; }
        void neg() { signal = -signal; }
    // << Métodos de escrita em ostream >>
        void big2text(const char *, size_t len, size_t dim);
        void big2hex(const char *, size_t len, size_t dim);
        void big2dec(const char *, size_t len, size_t dim);
    // << Metodos de conversão >>
        void text2big (const char *, size_t len, size_t dim);
        void hex2big (const char *, size_t len, size_t dim);
        void dec2big (const char *, size_t len, size_t dim);
    // << Sobrecarga dos operadores >>
        void operator++();
        void operator<<=(const word n)
            { if (n!= 0 && !isZero()) shiftLeft(n); }
        void operator>>=(const word n)
            { if (n!= 0 && !isZero()) shiftRight(n); }
        void operator+=(const BigRep&);
        void operator-=(const BigRep&);
        BigRep *operator* (const BigRep&) const;
        BigRep *operator% (const BigRep&) const;
        BigRep *operator/ (const BigRep&) const;
    #define oper(op) \
        void operator op (word);
        oper(+=) oper(-=) oper(*=) oper(/=) oper(%=)
    #undef oper
        // << Operadores Relacionais >>
    #define oper(op) \
        int operator op (const BigRep & b) const { \
            return cmp(*this, b) op 0; \
        }
        oper(<) oper(>) oper(<=) oper(>=) oper(!=) oper(==)
    #undef oper
    // << Gestão do contador de Referências >>
        void incRef() { ++count;                                            }
        int decRef() { if(--count == 0) return count; delete this; return;  }
        bool unique() const { return count == 1;                            }
    };
    class BigRepPtr{    // Apontador inteligente para BigRep
    // << Atributos >>
        BigRep * b;     // Apontador para representação
    public:
    // << Construtores >>
        BigRepPtr(BigRep * x) : b(x)                { b->incRef();  }
        BigRepPtr(const BigRepPtr & x) : b( x.b )   { b->incRef();  }
        ~BigRepPtr()                                { b->decRef();  }
    // << Operadores afetação >>
        void operator=(BigRep * x)  { x->incRef(), b->decRef(); b=x;}
        void operator=(const BigRepPtr & x)         { *this = x.b;  }
    // << Operadores de desreferência >>
        BigRep &operator*() const                   { return *b;    }
        BigRep *operator->() const                  { return b;     }
    };
// << Atributo único >>
    BigRepPtr brp;      // Apontador inteligente para BigRep
// << Métodos Auxiliares >>
    void unlink();
public:
// << Construtores >>
    // Construtor com string C-style e dimensão
    BigInt(const char *, StrType = DEC, size_t = DIM_MIN);
    // Construtor com valor tipo long e dimensão
    BigInt(long = 0, size_t = DIM_MIN);
    // Construtor com valor tipo BigInt e dimensão
    BigInt(size_t dimension, const BigInt & x);
    // Construtor a partir de um temporário
    BigInt(const BigTmp &x);
    // ... Restantes membros (interface BigInt)
    BigInt &operator=(long n);
// << Métodos de escrita no ostream
    // .....
// << Métodos de Acesso >>
    size_t size() const     { return brp->sz;   }
    BigInt operator-() const{
        BigInt aux(*this); aux.brp->neg(); return aux;
    }
    BigInt operator++() {
        unlink(); ++(*brp); return *this;
    }
// << Operadores Aritméticos >>
#define oper(op) \
    BigInt &operator op (const BigInt &b) \
       { unlink(); *brp op *b.brp; return *this; }
    oper(+=) oper(-=)
#undef oper
#define oper(op) \
    BigInt &operator op##= (const BigInt &b) \
        { brp = *brp op *b.brp; return *this; }
    oper(*) oper(/) oper(%)
#undef oper

#define oper(op) \
    friend BigInt operator op (const BigInt &a, const BigInt &b) \
        { BigInt aux(a); return aux op##= b; }
    oper(+) oper(-) oper(*) oper(/) oper(%)
#undef oper

#define oper(op) \
    friend bool operator op (const BigInt &a, const BigInt &b) \
        { return *a.brp op *b.brp; }
    oper(<) oper(>) oper(<=) oper(>=) oper(!=) oper(==)
#undef oper
};

class BigTmp{       // BigInt temporário
    friend class BigInt;
// << Atributos >>
    BigInt bigint; // Unico Atributo
    BigTmp(size_t dim, const BigInt &x) : bigint(dim, x) { }
public:
// << sobrecarga dos operadores aritméticos >>
#define oper(op) \
    BigTmp &operator op (const BigInt &x) \
        { bigint op##= x; return *this; }
    oper(+) oper(-)
#undef oper
};

#define oper(op) \
    inline BigTmp &operator op (const BigInt &a, const BigInt &b) \
        { return BigTmp(max(a.size(), b.size())+1, a) op b; }
    oper(+) oper(-)
#undef oper

#define oper(op) \
    inline BigTmp &BigTmp::operator op (const BigInt &x) \
        { bigint op##= x; return *this; }
    oper(+) oper(-)
#undef oper

inline BigInt::BigInt(const char *s, StrType type, size_t dim) : brp( new BigRep(s, type, dim )) { }
inline BigInt::BigInt(long n, size_t dim) : brp ( new BigRep(n, dim) ) { }
inline BigInt::BigInt(size_t dim, const BigInt &x) : brp( new BigRep(*x.brp, dim) ) { }
inline BigInt::BigInt(const BigTmp &tmp) : brp (tmp.bigint.brp) { }

BigInt & BigInt::operator=(long n) {
    if (brp->count > 1) brp = new BigRep(n);
    else *brp = n;
    return *this;
}
inline void BigInt::unlink() {
    if(brp->count > 1)
        brp = new BigRep(*brp);     // O segundo parametro toma
}                                   // valor DIM_MIN por omissão

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
// Método estatico que comapra em modulo objectos BigRep
int BigInt::BigRep::cmpMod(const BigRep &a, const BigRep &b) {
    if (a.sz != b.sz) return a.sz > b.sz ? 1:-1;
    size_t index = a.sz -1;
    for (;a.v[index]==b.v[index]; --index)
        if(index == 0) return 0;
    return (a.v[index]>b.v[index]) ? 1 : -1;
}
void BigInt::BigRep::operator= (long n) {
    if (n >= 0) signal = 1;
    else{ n = -n; signal = -1; }
    sz = (dword(n) > MAX_DIGIT ? 2 : 1);    // DIM_MIN >= 2
    v[0] = word (n%B_Radix);
    if (sz == 2)
        v[1] = word(n/B_Radix);
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
int main(int argc, char ** argv){
    std::cout << sizeof(byte) << " " << sizeof(word) << " " << sizeof(dword) << std::endl;
    std::cout << BITS_WORD << " " << B_Radix << " " << MAX_DIGIT << std::endl;
    return 0;
}