1.<br>
    a) Analisar e corrigir a seguinte definição de uma classe **Fraction**<br>
```cpp
class Fraction{
    int nn, dd;
public:
    Fraction(int n=0, int d=1): nn(n), dd(d) { normalize(); }
    Fraction *operator=( const Fraction &f ){
        dd = f.getDen(); nn = f.getNum(); return this;
    }
    int getNum() { return nn; }
    int getDen() { return dd; }
    Fraction &operator+=(const Fraction &f ){
        if ( dd != f.getDen() ) {
            nn*= f.getDen(); f.nn*= f.getDen(); dd*= f.getDen();
        }
        nn+= f.getNum(); normalize();
        return this;
    }
    Fraction &operator-( const Fraction &f ){
        Fraction aux; aux = *this; aux-=f; return aux;
    }
};

Fraction &operator-= ( Fraction &f1, const Fraction &f2) {
    f1 = f1-f2; return f1;
}

Fraction &operator+( const Fraction &f1, const Fraction &f2 ) {
    return Fraction(f1.nn*f2.dd + f2.nn*f1.dd, f1.dd*f2.dd);
}
```
b)  Definir o método *normalize()* para reduzir a fracção à expressão mais
simples, ou seja, sem divisores comuns ao numerador e denominador e com o
denominador positivo. Isto implica calcular o máximo divisor comum entre o
numerador e denominador, e deixar o sinal no numerador.

c)  Definir os métodos globais necessários à seguinte função *main()*
que calcula a soma dos primeiros 20 termos da sério com termo geral:<br>
<img src="https://latex.codecogs.com/svg.latex?{\color{Teal}\frac{(-1)^{(n-1)}*1}{n}}" title="{\color{Teal}\frac{(-1)^{(n-1)}*1}{n}}" />

```cpp
int main( int argc, char ** argv) {
    const int N = 20;   // Número de termos
    Fraction res = 0;   // Resultados parciais.
    for(int n=1; n <= N; ++n) {
        if (n % 2) res += Fraction(1, n);
        else res -= Fraction(1, n);
        std::cout << std::setw(2) << n << ". Resultado = " << std::setw(20) << std::setiosflags(ios::left) << res
        << " (" << (double) res << ')' << std::endl;
    }
    return 0;
}
```
d) Alterar a função para que o número de termos seja obtido através da linha de
comando.

e) Definir como público a sobrecarga do operador de comparação ("<"). Defina
a sobrecarga dos restantes operadores de comparação ("==", ">", "!=" )
invocando direta ou indiretamente o operador "<". Faça um programa para
teste dos operadores de comparação

f) Usando a classe Fraction, tente calcular a soma dos 100 primeiros termos
da série proposta na alínea c) e infira qual a razão pela qual esse objetivo não
é possível.
Derivado ao teto máximo de inteiros, não é possível calcular com exatidão o resultado.

2. Analise as seguintes definições
```cpp
class Time {
public:
    enum { HOURS = 24, MINUTES = 60 };
    typedef unsigned char TimeValue;
    Time(TimeValue h=0, TimeValue m=0) { hh=h; mm=m; normalize(); }
    Time operator-=(const Time &t ) const {
        if (*this < t) hh+= HOURS;
        if (mm < t.mm) { --hh; mm+= MINUTES; }
        hh -= t.hh; mm -= t.mm; return *this;
    }
    Time &operator-(const Time &t ) {
        Time aux(*this); aux -= t; return aux;
    }
    TimeValue getHours()        { return hh; }
    TimeValue getMinutes()      { return mm; }
    void setHours(TimeValue h)  { hh = h % HOURS; }
    void setMinutes(TimeValue m){ mm = m % MINUTES; }
private:
    unsigned char hh, mm;
};

Time operator+( const Time &t1, const Time &t2) const {
    return Time(t1.hh+t2.hh, t1.mm+t2.mm);
}

Time &operator+=(Time &t1, const Time &t2) {
    t1 = t1 + t2; return t1;
}

std::ostream &operator<<(std::ostream &o, const Time &t) {
    char cFill = o.fill();
    return o << std::setfill('0') << std::setw(2) << t.getHours() << ':' << std::setw(2) << t.getMinutes() << std::setfill(cFill);
}
```
a) Identifique as incorreções e emende-as

b) Defina como privado o método *normalize()*, e como públicos os *overloads* dos operadores de comparação ("<",">","==" e "!=").

c) Defina os métodos e funções globais necessários para que a função *main()* definida a seguir, funcione corretamente.
```cpp
int main(int argc, char ** argv) {
    std::cout << "Escreva uma expressão da forma:\n"
    "\t<expressão>::<operando><operador><operando>' = '\n"
    "\t<operador> :: '+'|'-'\n"
    "\t<operando> :: <horas> |\n"
    "\t              <horas> ':' <minutos> |\n"
    "\t              ':' <minutos>" << std::endl;
    Time t1, t2; char op, igual;
    cin >> t1 >> op >> t2 >> igual;
    Time res;
    switch(op) {
        case '+': res = t1 + t2; break;
        case '-': res = t1 - t2;
    }
    std::cout << t1 << ' ' << op << ' ' << t2 << " = " << res << std::endl;
}
```
d) Acrescente ao programa anterior a possibilidade de calcular o módulo da diferença temporal entre duas instâncias da classe *Time* (use o símbolo "^" para significar esta operação e defina a respetiva função *overload*).
