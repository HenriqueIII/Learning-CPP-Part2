#include <iostream>
class Count {
    const unsigned mod;     // Módulo de contagem
    unsigned val;           // Valor Corrente.
public:
    Count(unsigned m, unsigned v=0):
    mod(m), val(v%m) {
        // Corpo vazio
    }
    void operator++() {
        if (++val >= mod )
            val = 0;
    }
    void operator--(){
        if (val-- == 0)
            val = mod - 1;
    }
    unsigned operator() () const {
        return val;
    }
    void operator= (unsigned v){
        val = v % mod;
    }
};

int main(int argc, char ** argv){
    Count c1(10,1), c2(8);  // Criação dos objectos
    while (c1()) {
        ++c1; ++c2;   // Incremento dos contadores
        std::cout << "c1 = " << c1() << " c2 = " << c2() << std::endl;
    }
    std::cout << std::endl;
    c2 = 5; // Afecta o valor ao contador.
    for (int i = 5; i; i--) {
        --c1; --c2;   // Decremento dos contadores
        std::cout << "c1 = " << c1() << " c2 = " << c2() << std::endl;
    }
    return 0;
}