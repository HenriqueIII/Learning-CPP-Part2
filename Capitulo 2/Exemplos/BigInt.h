#ifndef BIG_INT_H
#define BIG_INT_H

#include <iostream>
#include <cstring>

// <<Definição de tipos >>
typedef unsigned char byte;     // sizeof == 8bits
typedef unsigned short word;    // sizeof == 16bits
typedef unsigned int dword;     // sizeof == 32bits

// << Definição de constantes >>
static const size_t BITS_WORD = sizeof(word) * 8;
// Base de Numeração (65536)
static const dword B_Radix = 0xFFFF + 1;
// Maior algarismo na base B_Radix
static const word MAXDIGIT = 0xFFFF;

class BigInt {
public:
    // Enumerado de tipos de strings para o construtor
    enum StrType {TEXT, HEX, DEC};
private:
// << Classes internas >>
    class BigRep {       // Representação (Body)
    // << Atributos >>
        int count;      // BigInts que partilham este BigRep
        size_t sz;      // Número de digitos
        size_t dim;     // Dimensão do espaço alojado
        int signal;     // -1 se negativo, 1 se positivo
        word *v;        // A representação
    // << Metodos auxiliares >>
        // Reserva de Memória
        word *allocate(size_t dimension);
        word *allocate(size_t s, size_t d);
        void reserve(size_t newDimension);
        //Auxxiliar para escrita em decimal num ostream
        static void big2dec(BigRep & bb, std::ostream &os);
        //Metodo de comparação
        static int cmp(const BigRep & a, const BigRep & b);
        //Metodos de comparação em modulo
        static int cmpMod(const BigRep & a, const BigRep & b);
        static int cmpMod(const BigRep & a, word b);
        //Metodos executores das operações shift
        void shiftLeft(const word nShifts);
        void shiftRight(const word nShifts);
        //Metodos executores das operações normalizadas
        static void addNorm(BigRep &add, const BigRep &max, const BigRep &min);
        static void addNorm(BigRep &add, const BigRep &br, word);
        static void subNorm(BigRep &sub, const BigRep &max, const BigRep &min);
        static void subNorm(BigRep &sub, const BigRep &max, word);
        static void multNorm(BigRep &prod, const BigRep &max, const BigRep &min);
        static void multNorm(BigRep &prod, const BigRep &br, word);
        //Metodo auxiliar para a divisão normalizada
        static word gessDig (BigRep &aux, BigRep &d);
        static void divNorm(BigRep &quo, BigRep &rest, const BigRep &dd, const BigRep &d);
        static word divNorm(BigRep &quo, word &rest, const BigRep &dd, word);
    public:
        friend BigInt;
    // << Construtores e destrutores >>
        BigRep (const char * s, StrType, size_t = DIM_MIN);
        BigRep (long = 0, size_t = DIM_MIN);
        BigRep (const BigRep &, size_t dimension=DIM_MIN);
        ~BigRep () { delete [] v; }
    // << Operadores afetação >>
        void operator=(const BigRep &);
        void operator=(const long);
    // << Metodos de acesso >>
        bool isOdd()const  { return (v[0] & 1); }
        bool isEven()const { return !(v[0] & 1); }
        bool isZero()const { return (sz == 1 && v[0] == 0); }
        bool isOne()const  { return sz == 1 && v[0] == 1 && signal == 1; }
        void neg() { signal = -signal; }
    // << Metodos de escrita em stream >>
        void big2txt(std::ostream &os) const; // BigInteger to text
        void big2dec(std::ostream &os) const; // BigInteger to dec
        void big2hex(std::ostream &os) const; // BigInteger to hex
    // << Metodos de conversão >>
        void text2big (const char *, size_t len, size_t dim);
        void hex2big (const char *, size_t len, size_t dim);
        void dec2big (const char *, size_t len, size_t dim);
    // << Sobrecarga dos Operadores >>
        void operator++();
        void operator <<=(const word n)
            { if (n!=0 && !isZero()) shiftLeft(n); }
        void operator >>=(const word n)
            { if (n!=0 && !isZero()) shiftRight(n); }
        void operator += (const BigRep&);
        void operator -= (const BigRep&);
        BigRep *operator* (const BigRep&) const;
        BigRep *operator% (const BigRep&) const;
        BigRep *operator/ (const BigRep&) const;
    #define oper(op)\
        void operator op (word);
        oper(+=) oper(-=) oper(*=) oper(%=) oper(/=)
    #undef oper
    // << Operadores relacionais >>
    #define oper(op)\
        int operator op (const BigRep &b) const {\
            return cmp(*this, b) op 0; \
        }
        oper(<) oper(>) oper(<=) oper(>=) oper(!=) oper(==)
    #undef oper
    // << Metodos de acesso à contagem de referências >>
        void incRef() { ++count; };
        int decRef() { 
            if(--count) 
                return count; 
            delete this;
            return count;
        }
        bool unique() { return count == 1; }
    };

    class BigRepPtr {   // Apontador inteligente para BigRep
    // << Membros >>
        BigRep * ptr;   // Ponteiro para representação.
    public:
    // << Construtores e destrutor >>
        BigRepPtr(BigRep * sr) : ptr(sr) { ptr->incRef(); }
        BigRepPtr(const BigRepPtr &srp) : ptr(srp.ptr) { ptr->incRef(); }
        ~BigRepPtr() { ptr->decRef(); }
    // << Operadores afetação >>
        void operator= (BigRep *s){
            s->incRef(); ptr->decRef(); ptr = s;
        }
        void operator= (const BigRepPtr &sp) {
            *this = sp.ptr;
        }
    // << Operadores de desreferencia >>
        BigRep &operator* () const { return *ptr; }
        BigRep *operator-> () const { return ptr; }
    };
    
    class BigTmp;       // BigInt Temporário

// << Atributo Unico >>
    BigRepPtr brp;
    // Capacidade minima do vector representado
    static const byte DIM_MIN = 4;
// << Metodos auxiliares >>
    void unlink() {
        if (brp->count > 1)
            brp = new BigRep(*brp); // o segundo parametro toma
    }                                // valor DIM_MIN por omissão
public:
// << Construtores >>
    // Construtor com string C-Style e dimensão
    BigInt (const char *, StrType = DEC, size_t = DIM_MIN);
    // Construtor com valor tipo long e dimensão
    BigInt (long = 0, size_t = DIM_MIN);
    // Construtor com valor tipo BigInt e dimensão
    BigInt (size_t dimension, const BigInt &x);
    // Contrutor a partir de um temporário
    BigInt (const BigTmp &x);
// << Interface de BigInt >>
    BigInt &BigInt::operator=(long n) {
        if (brp->count > 1)
            brp = new BigRep(n);
        else
            *brp = n;
        return *this;
    }
// << Metodos de acesso >>
    BigInt operator-() const {
        BigInt aux(*this); aux.brp->neg(); return aux;
    }
// << Operadores aritmeticos >>
    #define oper(op) \
    BigInt &operator op (const BigInt &b) {\
        unlink(); *brp op *b.brp; return *this;\
    }
    oper(+=) oper(-=)
    #undef oper

    #define oper(op)\
    BigInt &operator op##= (const BigInt &b) {\
        brp = *brp op *b.brp; return *this;\
    }
    oper(*) oper(/) oper(%)
    #undef oper

};

class BigInt::BigTmp {

};



/********************************************
*         CONSTRUTORES BIGINT               *
********************************************/

inline BigInt::BigInt (const char * s, StrType type, size_t dim)
: brp (new BigRep(s, type, dim)) { }

inline BigInt::BigInt (long n, size_t dim)
: brp (new BigRep(n, dim)) { }

inline BigInt::BigInt (size_t dim, const BigInt & x)
: brp (new BigRep(*x.brp, dim)) { }

inline BigInt::BigInt (const BigTmp & tmp)
: brp (tmp.bigint.brp) { }

/***************************************
*       CONSTRUTORES BIGREP            *
***************************************/

BigInt::BigRep::BigRep (const char *s, StrType type, size_t dim) {
    switch (type) {
        case TEXT: text2big(s, strlen(s), dim); break;
        case  HEX: hex2big(s, strlen(s), dim); break;
        case  DEC: default: dec2big(s, strlen(s), dim);
    }
    count = 0;
}

BigInt::BigRep::BigRep (long n, size_t dim) {
    count = 0;      // Iniciar contador de referências
    if (n >= 0)
        signal = 1;
    else
        { n = -n; signal = -1; }

    sz = (dword(n) > MAXDIGIT) ? 2 : 1;
    v = allocate(dim);
    v[0]= word (n % B_Radix);
    if (sz == 2)
        v[1] = word(n / B_Radix);
}

/***************************************
*       METODOS BIGREP                 *
***************************************/

word *BigInt::BigRep::allocate (size_t dimension) {
    for (dim = DIM_MIN; dim < dimension; dim *= 2);
    return new word [dim];
}

word *BigInt::BigRep::allocate (size_t s, size_t d) {
    return allocate (d < s ? s : d);
}

void BigInt::BigRep::text2big (const char * s, size_t len, size_t dim) {
    // Calculo da dimensão minima do array de words
    sz = len / sizeof(word) + (len % sizeof(word) != 0);
    // Reserva espaço maior que sz e menor ou igual a dim
    v = allocate(sz, dim);
    v[sz-1] = 0;            // Garantir zero nos bits nao usados
    signal = 1;             // Positivo
    memcpy(v,s,len);        // copia os dados
}

void BigInt::BigRep::hex2big(const char * s, size_t len, size_t dim) {
    const char * saux = s;
    const size_t BITS_PER_DIGIT = 4;
    const size_t DIGITS_PER_WORD = BITS_WORD/BITS_PER_DIGIT;
    if ( *saux == '-' ) {
        signal = -1;
        ++saux;
    }else
        signal = 1;
    while (*saux == '0' ) ++saux;   // Remoção zeros à esquerda
    len -= saux - s;                // Atualização do comprimento
    // Calculo da dimensão minima do array de words
    sz = len / DIGITS_PER_WORD + (len%DIGITS_PER_WORD != 0);
    v = allocate(sz, dim);          // Reserva do espaço
    v[sz -1] = 0;                   // Garante zero nos bits não usados
    // Numero de digitos a converter para word
    size_t nOfDigs = DIGITS_PER_WORD;
    --len;
    for (size_t i = 0; len; ++i, len -= nOfDigs) {
        if (len < DIGITS_PER_WORD)  // Ultimo conjunto a converter
            nOfDigs = len;
        // Converter digs caracteres em word
        word aux = 0;
        for (size_t j = nOfDigs; j; --j) {
            aux <<= BITS_PER_DIGIT;
            aux += tolower(saux[len -j]) - (isdigit(saux[len -j]) ? '0' : 'a' - 10 );
        }
        v[i] = aux;
    }
}

void BigInt::BigRep::dec2big(const char * s, size_t len, size_t dim){
    int aux_signal;
    if (*s == '-')
        { aux_signal = -1; ++s; }
    else
        aux_signal = 1;

    while (*s == '0') ++s;          // Remover os zero à esquerda

    v = allocate(len/4,dim);        // Número de words é len/4
    sz = 1; v[0] = 0; signal = 1;   // Colocar *this = 0
    while (*s) {
        *this *= 10;
        *this += *s++ -'0';
    }
    signal = aux_signal;            // Atualizar o sinal
}

void BigInt::BigRep::operator= (long n) {
    if (n >= 0) signal = 1;
    else { n = -n; signal = -1; }

    sz = (dword(n) > MAXDIGIT) ? 2 : 1; 

    v[0] = word (n % B_Radix);

    if (sz == 2)
        v[1] = word (n / B_Radix);
}



#define oper(op) \
    BigInt operator op (const BigInt &a, const BigInt &b) {\
        BigInt aux(a); return aux op##= b;\
    }
    oper(+) oper(-) oper(*) oper(/) oper(%)
    #undef oper

#endif