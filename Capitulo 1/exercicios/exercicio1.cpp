#include <iostream>
#include <math.h>

class Fraction{
    int nn, dd;
    void normalize(){
        int mdc,a=nn,b=dd;
        if (a>b){
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
    Fraction *operator=( const Fraction &f ){
        dd = f.getDen(); nn = f.getNum(); return this;
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


int main(){
    Fraction a(-4,-1);
    std::cout << a << std::endl;
}