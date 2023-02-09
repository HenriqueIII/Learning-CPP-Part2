#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>

// << Definição de tipos >>
typedef unsigned char byte;     // sizeof == 8 bits == 255
typedef unsigned short word;    // sizeof == 16 bits == 65535
typedef unsigned long dword;    // sizeof == 32 bits == 4 294 967 295

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
        /*static int cmpMod(const BigRep &a, word b);*/
        // Métodos executores das operações shift
        void shiftLeft(const word nShifts);
        void shiftRight(const word nShifts);
        // Métodos executores das operações normalizadas
        static void addNorm(BigRep &add, const BigRep &max, const BigRep &min);
        /*static void addNorm(BigRep &add, const BigRep &br, word);*/
        static void subNorm(BigRep &sub, const BigRep &max, const BigRep &min);
        static void subNorm(BigRep &sub, const BigRep &max, word);
        static void multNorm(BigRep &prod, const BigRep &max, const BigRep &min);
        static void multNorm(BigRep &prod, const BigRep &br, word);
        // Método auxiliar para a divisão normalizada
        static word gessDig (BigRep &aux, BigRep &d);
        static void divNorm(BigRep &quo, BigRep &rest, const BigRep &dd, const BigRep &d);
        static void divNorm(BigRep &quo, word &rest, const BigRep &dd, word);
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
        void big2txt(std::ostream &) const;
        void big2hex(std::ostream &) const;
        void big2dec(std::ostream &) const;
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
        int decRef() { if(--count) return count; delete this; return count; }
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
    void big2dec(std::ostream &os) const { brp -> big2dec(os); }
    void big2hex(std::ostream &os) const { brp -> big2hex(os); }
    void big2txt(std::ostream &os) const { brp -> big2txt(os); }
// << Métodos de Acesso >>
    size_t size() const     { return brp->sz;   }
    size_t capacity() const { return brp->dim;  }
    bool isOdd() const  { return brp->isOdd();  }
    bool isEven() const { return brp->isEven(); }
    bool isZero() const { return brp->isZero(); }
    bool isOne() const  { return brp->isOne();  }
    BigInt &neg()       { unlink(); brp->neg(); return *this;}
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
    inline BigInt &operator op (word n) \
        { unlink(); *brp op n; return *this; }
    oper(<<=) oper(>>=)
#undef oper

#define oper(op) \
    friend BigInt operator op (const BigInt &a, const BigInt &b) \
        { BigInt aux(a); return aux op##= b; }
    oper(*) oper(/) oper(%)
#undef oper

#define oper(op) \
    friend bool operator op (const BigInt &a, const BigInt &b) \
        { return *a.brp op *b.brp; }
    oper(<) oper(>) oper(<=) oper(>=) oper(!=) oper(==)
#undef oper

#define oper(op) \
    BigInt &operator op (const long);
    oper(+=) oper(-=) oper(*=) oper(/=) oper(%=)
#undef oper
};

inline size_t max(size_t a, size_t b) { return (a>b) ? a : b; }

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
#define oper(op) \
    friend BigTmp operator op ( const BigInt &a, const BigInt &b);
    //    { return BigTmp(max(a.size(), b.size())+1, a) op b; }
    oper(+) oper(-)
#undef oper 
};
BigTmp operator+ (const BigInt&a, const BigInt&b);
BigTmp operator- (const BigInt&a, const BigInt&b);
inline void BigInt::unlink() {
    if(brp->count > 1)
        brp = new BigRep(*brp);     // O segundo parametro toma
}   
std::ostream &txt(std::ostream &o);
std::ostream &operator<<(std::ostream &os, const BigInt &bi);
inline BigInt::BigInt(const char *s, StrType type, size_t dim) : brp( new BigRep(s, type, dim )) { }
inline BigInt::BigInt(long n, size_t dim) : brp ( new BigRep(n, dim) ) { }
inline BigInt::BigInt(size_t dim, const BigInt &x) : brp( new BigRep(*x.brp, dim) ) { }
inline BigInt::BigInt(const BigTmp &tmp) : brp (tmp.bigint.brp) { }

#endif