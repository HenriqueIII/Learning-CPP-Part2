#include "Fraction.h"
#include <iostream>

int main (int argc, char ** argv ) {
    const BigInt desn(exp10at(55)); // Dez elevados a 55.
    const BigInt unit(1);           // Unidade.
    long n; Fraction res(0L);
    BigInt d(1L), decimalPrev, decimalCurr;
    for (n = 1; n < 5 || (decimalPrev < decimalCurr); ++n) {
        decimalPrev = decimalCurr;
        res += Fraction(unit, d); d*=n;
        // Parte inteira dos resultados parciais a 55 digitos.
        BigInt integer(res.getNum() / res.getDen());
        // Parte decimal dos resultados parciais a 55 digitos.
        BigInt aux((res.getNum()*desn) / res.getDen());
        decimalCurr = (aux - integer * desn);
        // Resultados parciais em virgula fixa
        std::cout << "Parcial =" << integer << '.' << decimalCurr << std::endl;
    }
    std::cout << "O valor " << res.getNum() / res.getDen() << '.' << decimalCurr << " foi obtido em "
    << n << " passos." << std::endl; 
}