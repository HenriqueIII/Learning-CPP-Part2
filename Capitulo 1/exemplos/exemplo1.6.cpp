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
    friend Complex operator+ (const Complex &, const Complex &);

    friend std::ostream &operator<< (std::ostream &, const Complex &);
};
inline Complex operator+ (const Complex &c1, const Complex &c2) {
    return Complex (c1.re + c2.re, c1.im + c2.im);
}
// Sobrecarga do operador << como funçao global
inline std::ostream &operator<<(std::ostream & out, const Complex & c) {
    return out << c.re << (c.im >= 0 ? '+' : '-' ) << 'j' << fabs(c.im);
}

int main (int argc, char ** argv) {
    Complex c1 (1 , 2), c2(3), c3;
    c3 = c1 + c2;
    std::cout << c3 << std::endl;
    c3 = c1 + 3; // Equivalente a c3 = c1.operator+(Complex(3))
    std::cout << c3 << std::endl;
    c3 = 3 + c1;
     return 0;
}