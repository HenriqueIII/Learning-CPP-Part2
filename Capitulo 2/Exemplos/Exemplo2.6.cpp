#include <iostream>

// <<Definição de tipos>>

typedef unsigned char byte;     // sizeof == 8 bits
typedef unsigned short word;    // sizeof == 16 bits
typedef unsigned int dword;    // sizeof == 32 bits

// <<Definição de constantes>>

static const size_t BITS_WORD = sizeof(word) * 8;

// Base de numeração (65536)
static const dword B_Radix = 0xFFFFL + 1;

// Maior algarismo na base B_Radix == (B_Radix -1)

static const word MAXDIGIT = 0xFFFF;

class BigInt {
// << Classes internas >>
    class BigRep;       // Representação (body)
    class BigRepPtr;    // Apontador inteligente para BigRep
    class BigTmp;       // BigInt Temporário

// << Atributo Único >>
    BigRepPtr brp;      // Apontador inteligente para BigRep

};

class BigInt::BigRep {  // Body, representação de um BigInt
    int count;      // BigInts que partilham este BigRep
    size_t sz;      // Número de digitos
    size_t dim;     // Dimensão do espaço alojado
    int signal;     // -1 se negativo, 1 se positivo
    word *v;        // A representação          
};



int main(int argc, char ** argv) {
    
    return 0;
}