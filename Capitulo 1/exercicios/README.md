1.<br>
    a) Analisar e corrigir a seginte definição de uma classe **Fraction**<br>
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
a sobecarga dos restantes operadores de comparação ("==", ">", "!=" )
invocando direta ou indirectamente o operador "<". Faça um programa para
teste dos operadores de comparação

f) Usando a classe Fraction, tente calcular a soma dos 100 primeiros termos
da série proposta na alinea c) e infira qual a razão pela qual esse objectivo não
é possível.
