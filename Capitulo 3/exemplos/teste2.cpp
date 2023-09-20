#include <iostream>

class B{
public:
    virtual char * f() { return (char *) "B::f()"; } // Polimorfa
            char * g() { return (char *) "B::g()"; } // Normal
};

class D: public B {
public:
    char * f() { return (char *) "D::f()"; } // Polimorfa
    char * g() { return (char *) "D::g()"; } // Normal
};

int main(int argc, char ** argv) {
    D obj;
    B *pt = &obj;   // pt é do tipo de apontador para B
                    // mas aponta para um objecto D
    std::cout << pt -> f() << std::endl;
    std::cout << pt -> g() << std::endl;
    return 0;
}