#include <iostream>
#include <math.h>

class Complex{
    double re, im; // Parte real e parte imaginaria
public:
    // Construtor
    Complex(double r, double i){
        re = r;
        im = i;
    }
    // Sobrecarga do operador +
    Complex operator+ (const Complex &c) {
        return Complex(re + c.re, im + c.im);
    }
    // Sobrecarga do operador -
    Complex operator- (const Complex &c) {
        return Complex(re - c.re, im - c.im);
    }
    // Sobrecarga do operador *
    Complex operator* (const Complex &c) {
        return Complex(re * c.re - im * c.im, re * c.im + im * c.im );
    }
    friend std::ostream &operator<< (std::ostream &, const Complex &);
};
// Sobrecarga do operador << como funçao global
std::ostream &operator<<(std::ostream & out, const Complex & c) {
    return out << c.re << (c.im >= 0 ? '+' : '-' ) << 'j' << fabs(c.im);
}

int main (int argc, char ** argv) {
    // Definição e iniciação de complexos.
    Complex a(1.2, 3), b(6.5, 4);
    Complex c = b;  // Instanciar c por copia de b
    a = b + c;      // Afectar a com a soma de b com c
    b = b + c * a;
    // Afectar c com (a-b) somado com o complexo (1+j2)
    c = a - b + Complex (1, 2);
    // Inserir no ostream o complexo resultante
    std::cout << c << std::endl;
    return 0;
}