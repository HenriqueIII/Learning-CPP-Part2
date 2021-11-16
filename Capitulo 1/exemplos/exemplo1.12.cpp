#include <iostream>
#include <math.h>

class Complex {
    double re, im;
public:
    Complex() { re = im = 0; }
    Complex( double rr, double ii = 0) { re = rr; im = ii ;}
    // Sobrecarga da soma com retorno por referência.
    friend Complex &operator+(const Complex&, const Complex&);
    friend Complex &operator-(const Complex&, const Complex&);
    friend std::ostream &operator<< (std::ostream &, const Complex&);
};

// Função global para obter uma referência para um complexo.
Complex &getTmp() {
    enum { MaxTmp = 16 };
    static int n = 0;
    static Complex buffer[MaxTmp];
    if (n == MaxTmp) n = 0;
    return buffer[n++];
}
// Função global sobrecarga do operador '+' entre complexos.
Complex &operator+(const Complex &c1, const Complex &c2) {
    Complex &result = getTmp();
    result.re = c1.re + c2.re;
    result.im = c1.im + c2.im;
    return result;
}
// Função global sobrecarga do operador '-' entre complexos.
Complex &operator-(const Complex &c1, const Complex &c2) {
    Complex &result = getTmp();
    result.re = c1.re - c2.re;
    result.im = c1.im - c2.im;
    return result;
}

inline std::ostream &operator<< (std::ostream &o, const Complex &c){
    return o << c.re << (c.im >= 0 ? '+' : '-') << 'j' << fabs(c.im);
}

int main (int argc, char ** argv) {
    Complex res1, res2, c1(3, 5), c2(4, 7.1);
    // Uma soma e duas subtrações.
    res1 = c1 - c2 + 3 - Complex(5, -3.1);
    // Duas subtrações e uma soma
    res2 = res1 - c1 + c2 - 3 + Complex(5, -3.1);
    std::cout << "res1= " << res1 << "\nres2= " << res2 << "\n";
}