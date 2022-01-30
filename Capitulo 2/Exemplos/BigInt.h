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
// << Classes internas >>
    class BigRep;       // Representação (Body)
    class BigRepPtr;    // Apontador inteligente para BigRep
    class BigTmp;       // BigInt Temporário
// << Atributo Unico >>
    BigRepPtr brp;
    // Capacidade minima do vector representado
    static const byte DIM_MIN = 4;
public:
    // Enumerado de tipos de strings para o construtor
    enum StrType {TEXT, HEX, DEC};
// << Construtores >>
    // Construtor com string C-Style e dimensão
    BigInt (const char *, StrType = DEC, size_t = DIM_MIN);
    // Construtor com valor tipo long e dimensão
    BigInt (long = 0, size_t = DIM_MIN);
    // Construtor com valor tipo BigInt e dimensão
    BigInt (size_t dimension, const BigInt &x);
    // Contrutor a partir de um temporário
    BigInt (const BigTmp &x);
};

class BigInt::BigRep {  // Body, representação de um BigInt
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
public:
// << Construtores e destrutores >>
    BigRep (const char * s, StrType, size_t = DIM_MIN);

// << Metodos de conversão >>
    void text2big (const char *, size_t len, size_t dim);
    void hex2big (const char *, size_t len, size_t dim);
    void dec2big (const char *, size_t len, size_t dim);
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

/***************************************
*       METODOS BIGREP                 *
***************************************/

void BigInt::BigRep::text2big (const char * s, size_t len, size_t dim) {
    // Calculo da dimensão minima do array de words
    sz = len / sizeof(word) + (len % sizeof(word) != 0);
    // Reserva espaço maior que sz e menor ou igual a dim
    v = allocate(sz, dim);
    v[sz-1] = 0;            // Garantir zero nos bits nao usados
    signal = 1;             // Positivo
    memcpy(v,s,len);        // copia os dados
}

word *BigInt::BigRep::allocate (size_t dimension) {
    for (dim = DIM_MIN; dim < dimension; dim *= 2);
    return new word [dim];
}

word *BigInt::BigRep::allocate (size_t s, size_t d) {
    return allocate (d < s ? s : d);
}

#endif