// << Classe B derivada da classe A >>
#include <iostream>

// Classe base
class A {
    int x;      // Atributo privado, oculto na classe B.
public:
    void set10()    { x = 10;           }
    void show()     { std::cout << x;   }
};
class B : public A {    // Classe derivada de A
    char c;     // Atributo acrescentado
public:
    //void setA10()   {c = 'A'; x = 10;   } // ERRO: x oculto.
    void setA10()   {c = 'A'; set10();  }   // Metodo acrescentado
    void show()     { std::cout << c; A::show(); }  // Metodo alterado
};

int main(int argc, char** argv) {
    A a; B b;
    a.set10(); std::cout << "Show A - "; a.show(); std::cout << std::endl;
    b.setA10(); std::cout << "Show B - "; b.show(); std::cout << std::endl;
    a = b;      // Ok, um B tambem é um A
//  b = a;      // Erro, A não é um B
}