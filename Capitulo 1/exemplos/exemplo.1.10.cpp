#include <iostream>

struct Spy  // Classe auxiliar na construção de mensagens
{
    static int n1, n2, n3, n4;  // Contadores de ocorrencias
    Spy()   { reset(); }
    static void reset(){ n1 = n2 = n3 = n4 = 0; }
    static void print (const char *msg, int n) {
        if (n) {
            std::cout << "\t\t- " << msg << "foi chamado " << n << " vezes" << std::endl;
        }
    }
    static void display() { // Mostrar ocorrencias
        print("Test(char)", n1); print("Test(Test&)", n2);
        print("operator=()", n3); print("~Test()", n4);
    }
    ~Spy(){ display(); }
};

// Os atributos estaticos são iniciados a 0 por omissão
int Spy::n1, Spy::n2, Spy::n3, Spy::n4;


class Test  // Classe a analisar, tomada como tipica
{
    char c;
public:
    Test(char ch)   { Spy::n1++; c = ch;  }
    Test(Test &t)   { Spy::n2++; c = t.c; }
    ~Test()         { Spy::n4++;          }
    Test &operator= (const Test &t) {
        Spy::n3++; c = t.c; return *this;
    }
    Test &operator++ () { c++; return *this; }
    Test operator++ (int) {
        Test aux(*this); c++; return aux;
    }
};
// Funções com variantes de paramentros e retorno

Test f1(Test t) {return t;}
Test f2(Test &t) {return t;}
Test &f3(Test &t) {return t;}

// Programa principal
int main (int argc, char ** argv ){
    Spy spy;    // Objecto local a main()
    Test tt('A'), ttt(' ');
    std::cout << "Ao executar Test tt('A'), ttt(' ')\n";
    Spy::display();
    {   //Bloco de instruções locais ao módulo
        Spy spy;
        ttt = f1(tt);
        std::cout << "Ao executar ttt = f1(tt)\n";
    }
    {
        Spy spy;
        ttt = f2(tt);
        std::cout << "Ao executar ttt = f2(tt)\n";
    }
    {
        Spy spy;
        ttt = f3(tt);
        std::cout << "Ao executar ttt = f3(tt)\n";
    }
    {
        Spy spy;
        ttt = ++tt;
        std::cout << "Ao executar ttt = ++tt\n";
    }
    {
        Spy spy;
        ttt = tt++;
        std::cout << "Ao executar ttt = tt++\n";
    }
    Spy::reset();
    std::cout << "Na saida de main()\n";
    return 0;
}   // Chamada implicita ao destrutor de Spy.