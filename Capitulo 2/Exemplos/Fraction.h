#ifndef FRACTION_H
#define FRCTION_H

#include "BigInt.h"

class Fraction {
    BigInt num; // Numerador (positivo ou negativo)
    BigInt den; // Denominador (sempre positivo)
    // Simplicação da fracção com o MDC(num,den)
    void norm();
public:
    Fraction(long a) : num(a), den(1)       { }
    Fraction(BigInt &a) : num(a), den(1)    { }
    Fraction (const BigInt &a, const BigInt &b) : num(a), den(b) { }
    const BigInt &getNum() const { return num; }
    const BigInt &getDen() const { return den; }

    #define oper(op) \
        Fraction &operator op(const Fraction &b);
    oper(+=) oper(-=) oper(/=) oper(*=)
    #undef oper

    Fraction operator-() const
        { Fraction x = *this; x.num.neg(); return x; }

    #define oper(op) \
        Fraction operator op(const Fraction &b) const \
        { return Fraction(*this)op##=b; }
    oper(+) oper(-) oper(*) oper(/)
    #undef oper

    friend std::ostream & operator << (std::ostream &out, const Fraction &f)
    { return out << f.num << "/" << f.den; }
};

void Fraction::norm() {
    if(num.isZero())                { den=1; return;            }
    if(num.isOne() || den.isOne() ) { return;                   }
    if(num == den )                 { num = den = 1; return;    }
    if(den < 0L)                    { num.neg(); den.neg();     }
    // Algoritmo de Euclides
    BigInt n = num, d = den, MDC;
    while(!d.isZero()) { MDC = n % d; n = d; d = MDC; }
    MDC = n;
    // Fim do algoritmo de Euclides
    if(MDC.isOne()) return;
    num /= MDC; den /= MDC;
}

#define oper(op) \
    Fraction &Fraction::operator op##= (const Fraction &b) { \
    if (den == b.den) num op##= b.num;\
    else {\
        num = num * b.den op den * b.num; \
        den *= b.den; \
    }\
    norm();\
    return *this;\
    }
oper(+) oper(-)
#undef oper

Fraction &Fraction::operator*= (const Fraction &b) {
    num *= b.num;
    den *= b.den;
    norm();
    return *this;
}

Fraction &Fraction::operator/= (const Fraction &b) {
    num *= b.den;
    den *= b.num;
    norm();
    return *this;
}

BigInt one(1L), ten(10L);

BigInt exp10at (word n) {
    if(n==0) return one;
    BigInt v(ten);
    for (word i=1; i<n; ++i) v*=ten;
    return v;
}

#endif