#include <iostream>
#include "Fraction.h"
#include "BigInt.h"

int main(int, char**) {
    Fraction res = 0L, unit = 1;
    for (long n=1; n <= 400; ++n) {
        if (n % 2) res += unit / n;
        else res -= unit / n;
        BigInt intPart = res.getNum() / res.getDen();
        BigInt decPart = res.getNum() * 100000 / res.getDen() - intPart * 100000;
        std::cout << "Valor parcial = " << intPart << '.' <<decPart << std::endl;
    }
    std::cout << "Resultado = 0." << res.getNum()*100/res.getDen() << std::endl;
}