#include <iostream>
#include <math.h>
#include <iomanip>

class Fraction{
    int nn, dd;
    void normalize(){
        int mdc,a=nn,b=dd;
        if (a>b || a == 0){
            a = -(b - a);
            b = a + b;
            a = b - a;
        }
        for (mdc = abs(a); mdc > 1; --mdc) {
            if (a % mdc == 0 && b % mdc == 0)
                break;
        }
        nn = nn/mdc;
        if (dd<0)
            nn = -nn;
        dd = abs(dd/mdc);
    }
public:
    Fraction(int n=0, int d=1): nn(n), dd(d) { normalize(); }
    Fraction &operator=( const Fraction &f ){
        dd = f.getDen(); nn = f.getNum(); return *this;
    }
    int getNum() const { return nn; }
    int getDen() const { return dd; }

    Fraction &operator+=(const Fraction &f ){
        nn=(nn*f.getDen())+(f.getNum()*dd);
        dd*=f.getDen();
        normalize();
        return *this;
    }

    Fraction operator-( const Fraction &f ){
        return Fraction(nn*f.getDen() - f.getNum()*dd, f.getDen()*dd);
    }

    operator double() const{
        return double(nn)/double(dd);
    }

    bool operator< (const Fraction &f) const{
        return (double) *this < (double) f;
    }
};

std::ostream &operator<< (std::ostream &o, const Fraction &f){
    if (f.getDen() < 0 && f.getNum() > 0)
        o << '-';
    o << f.getNum();
    if (abs(f.getDen()) != 1)
        o << '/' << abs(f.getDen());
    return o;
}

Fraction &operator-= ( Fraction &f1, const Fraction &f2) {
    f1 = f1-f2; return f1;
}

Fraction operator+( const Fraction &f1, const Fraction &f2 ) {
    return Fraction(f1.getNum()*f2.getDen() + f2.getNum()*f1.getDen(), f1.getDen()*f2.getDen());
}

int power( int n, int p) {
        int res=n;
        for (int i = p; i>1; --i)
            res *= n;
        return res;
    }

int main( int argc, char ** argv) {
    /*if (argc != 2){
        std::cerr << "Utilização: programa <número de termos>" << std::endl;
        exit(-1);
    }
    const int N = std::atoi(argv[1]);   // Número de termos
    Fraction res = 0;   // Resultados parciais.
    for(int n=1; n <= N; ++n) {
        std::cout << std::setw(2) << n << ". Resultado " << res << " + " << Fraction(power(-1, n-1), n );
        res += Fraction (power(-1, n-1), n );
        std::cout << " = " << res << " (" << (double) res << ')' << std::endl;
    }*/
    Fraction f1(1,4), f2(1,2), f3(3,2), f4(-3,6);
    std::cout << f1 << " < " << f2 << " : " << (f1<f2?"Verdade":"Falso") << std::endl;
    std::cout << f3 << " < " << f2 << " : " << (f3<f2?"Verdade":"Falso") << std::endl;
    std::cout << f4 << " < " << f1 << " : " << (f4<f1?"Verdade":"Falso") << std::endl;
    std::cout << "1" << " < " << f1 << " : " << (1<f1?"Verdade":"Falso") << std::endl;  
    return 0;
}
