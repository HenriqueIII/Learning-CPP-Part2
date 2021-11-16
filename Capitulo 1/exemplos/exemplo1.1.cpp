#include <iostream>
class Count {
    const unsigned mod;     // Módulo de contagem
    unsigned val;           // Valor Corrente.
public:
    Count(unsigned m, unsigned v=0):
    mod(m), val(v%m) {
        // Corpo vazio
    }

    void incVal(){          // Incremento
        if (++val >= mod )
            val = 0;
    }

    void decVal(){          // Decremento
        if(val-- == 0)
            val = mod-1;
    }
    unsigned getVal() const {   // Retorna valor
        return val;
    }

    void putVal(unsigned v) {   // Afecta valor
        val = v % mod;
    }
};

int main(int argc, char ** argv){
    Count c1(10,1), c2(8);  // Criação dos objectos
    while (c1.getVal() != 0) {
        c1.incVal(); c2.incVal();   // Incremento dos contadores
        std::cout << "c1 = " << c1.getVal() << " c2 = " << c2.getVal() << std::endl;
    }
    std::cout << std::endl;
    c2.putVal(5); // Afecta o valor ao contador.
    for (int i = 5; i; i--) {
        c1.decVal(); c2.decVal();   // Decremento dos contadores
        std::cout << "c1 = " << c1.getVal() << " c2 = " << c2.getVal() << std::endl;
    }
    return 0;
}