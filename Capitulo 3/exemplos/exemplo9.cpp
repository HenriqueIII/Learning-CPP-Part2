#include <iostream>

class B {
    int n;
public:
    B(int nn): n(nn) { }
    int getn() { return n; }
    void display() { std::cout << "n= " << n << std::endl; }
};

class D: public B {
    char c;
public:
    D (int nn, int cc): B(nn), c(cc) { }
    //int getn1() {return n+c; }  // Erro. n é inacessivel em B.
    int getn() { return B::getn() + c ; }   // Ok. Invoca getn() de B
    // Oculta (override) B::display()
    void display() { std::cout << "n= "<<getn()<<", c= "<< int(c) <<std::endl; }
};

void f() {
    B b(7);
    D d( 5 , 4 );
    //std::cout << b.n;             // Erro. n é privado de B
    std::cout << d.B::getn() << std::endl;       // Invoca getn() de B.
    std::cout << d.getn() << std::endl;          // Invoca getn() de D
    b.display();                    // Invoca display() de B
    d.display();                    // Invoca display() de D
    B *ptB = &d;                    // Coerção implicita de apontadores
    ptB->display();                 // Invoca display() de B(ver nota no livro)    
}

int main(int argc, char ** argv) {
    f();
    return 0;
}