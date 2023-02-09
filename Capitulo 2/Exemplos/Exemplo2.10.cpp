#include <iostream>
#include "Fraction.h"

int main(int argc, char ** argv) {
    long n;
    Fraction    ka(one, BigInt(25L)),
                kb(one, BigInt(57121L)),
                kc(2L),
                a(BigInt(16L), BigInt(5L)),
                b(BigInt(4L), BigInt(239L)),
                c(one);
    
    const BigInt desn(exp10at(55));     // 10 Elevado a 55
    BigInt decimalPrev, decimalCurr; Fraction res(0L);
    for (n = 1; n<5 || (decimalPrev != decimalCurr); n++) {
        decimalPrev=decimalCurr;
        if(n%2) res += (a-b) / c;
        else res -= (a-b) /c;
        // Calculo da parte inteira e parte decimal dos
        // resultados parciais a 55 digitos
        // Calculo da parte inteira
        BigInt integer(res.getNum()/res.getDen());
        // Calculo da parte decimal
        BigInt aux(res.getNum()*desn /res.getDen());
        decimalCurr = aux - integer * desn;
        // Mostrar resultados parciais em virgula fixa
        std::cout << integer << '.' << decimalCurr << std::endl;
        a*=ka; b*=kb; c+=kc;    // Preparação do próximo ciclo
    }
    std::cout   << "O valor " << res.getNum() / res.getDen() << '.' << decimalCurr
                << " foi obtido em " << n << " passos." << std::endl;
}