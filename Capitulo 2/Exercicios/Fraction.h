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

    void showDec() const {
        std::cout << num/den << '.' << (num*10000000 / den) - (num/den) * 10000000;
    }
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

Fraction roundUp(Fraction x, word precision){
    BigInt integer, dec, desn(exp10at(precision));
    integer = x.getNum()/x.getDen();
    BigInt aux = x.getNum()*desn/x.getDen();
    dec = aux - integer * desn;
    if (!precision){
        aux = x.getNum()*exp10at(precision+1)/x.getDen();
        return Fraction(integer + (aux >= 5 ? 1 : 0), 1);
    }
    std::cout << integer << '.' << dec << std::endl;
    //return Fraction (x.getNum()/x.getDen(), x.getDen()*exp10at(precision) + (x.getDen()*exp10at(precision)>=5?1:0));
}

Fraction pi(){
    long n;
        Fraction    ka(one, BigInt(25L)),
                    kb(one, BigInt(57121L)),
                    kc(2L),
                    a(BigInt(16L), BigInt(5L)),
                    b(BigInt(4L), BigInt(239L)),
                    c(one);
        
        const BigInt desn(exp10at(8));     // 10 Elevado a 6
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
            a*=ka; b*=kb; c+=kc;    // Preparação do próximo ciclo
        }
    return res;
}

Fraction radian(word degree){
    return Fraction(degree) * pi() / 180;
}

Fraction degree(Fraction rad){
    return rad * 180 / pi();
}

Fraction sin(word degree, word precision) {      // Calcular o seno com precisão de x digitos
    switch (degree)
    {
    case 0:
        return Fraction(0L);
    case 30:
        return Fraction(1,2);
    case 90:
        return Fraction(1L);
    }
    bool f=1;
    const BigInt desn(exp10at(precision));     // 10 Elevado a 6
    BigInt decimalPrev, decimalCurr, d(1L); 
    Fraction r1(radian(degree));
    Fraction res(0L), rad(radian(degree));
    for (word i = 1; i < 5 || decimalPrev != decimalCurr; i+=2){
        if(i>1){
            rad=rad*r1*r1;
            d*=i-1; d*=i;
        }
        decimalPrev=decimalCurr;
        if (i%2) 
            if(f){
                res += rad / d; f=0;
            }else{
                res -= rad / d; f=1;
            }
        BigInt integer (res.getNum()/res.getDen()); 
        BigInt aux (res.getNum()*desn/res.getDen());
        decimalCurr = aux - integer * desn;
    }
    return res;
}

Fraction cos(word degree, word precision) {      // Calcular o coseno com precisão de x digitos
    switch (degree)
    {
    case 0:
        return Fraction(1L);
    case 60:
        return Fraction(1,2);
    case 90:
        return Fraction(0L);
    }
    bool f=0;
    const BigInt desn(exp10at(precision));     // 10 Elevado a precision
    BigInt decimalPrev, decimalCurr, d(1L); 
    Fraction r1(radian(degree));
    Fraction res(one), rad(radian(degree));
    for (word i = 1; i < 7 || decimalPrev != decimalCurr; i++){
        if(i>1){   
            rad*=r1;
            d*=i;
        }
        decimalPrev=decimalCurr;
        if (!(i%2)) 
            if(f){
                res += rad / d; f=0;
            }else{
                res -= rad / d; f=1;
            }
        BigInt integer (res.getNum()/res.getDen()); 
        BigInt aux (res.getNum()*desn/res.getDen());
        decimalCurr = aux - integer * desn;
    }
    return res;
}

Fraction arcsin(Fraction rad) {
    BigInt num(one), den(one);
    Fraction res(rad), d(1L), power(rad);
    const BigInt desn(exp10at(6)); 
    BigInt decimalPrev, decimalCurr;
    for (int i = 1; i < 5 || (decimalPrev!=decimalCurr) ; i++) {
        if(i%2){
            decimalPrev=decimalCurr;
            power=power*rad*rad;
            Fraction f1(power); f1/=i+2;
            num*=i;
            den*=i+1;
            Fraction f2(num,den);
            res+=(f1*f2);
            BigInt integer = res.getNum()/res.getDen();
            BigInt aux (res.getNum()*desn/res.getDen());
            decimalCurr = aux-integer*desn;
            std::cout << integer << '.' << decimalCurr << std::endl;
        }
    }
    return res;
}

#endif