#include <iostream>
#include <math.h>

class Complex{
    double re, im; // Parte real e parte imaginaria
public:
    // Construtor
    Complex(double r = 0, double i = 0){
        re = r;
        im = i;
    }
    // Sobrecarga do operador +
    Complex operator+ (const Complex &c) {
        return Complex(re + c.re, im + c.im);
    }
    friend std::ostream &operator<< (std::ostream &, const Complex &);
};
// Sobrecarga do operador << como funçao global
inline std::ostream &operator<<(std::ostream & out, const Complex & c) {
    return out << c.re << (c.im >= 0 ? '+' : '-' ) << 'j' << fabs(c.im);
}

int main (int argc, char ** argv) {
    Complex c1 (1 , 2), c2(3), c3;
    c3 = c1 + c2;
    std::cout << c3 << std::endl;
    c3 = c1.operator+ (c2);
    std::cout << c3 << std::endl;
    c3 = c1 + 3; // Equivalente a c3 = c1.operator+(Complex(3))
    std::cout << c3 << std::endl;
    //c3 = 3 + c1; Erro: c3 = 3.operator+(c1) é formalmente incorreto
    c3 = Complex(3) + c1; // Correcto devido a coerção explicita
    std::cout << c3 << std::endl;
    return 0;
}