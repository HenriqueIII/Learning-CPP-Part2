#include <iostream>
class Count {
    const unsigned mod;     // Módulo de contagem
    unsigned val;           // Valor Corrente.
public:
    Count(unsigned m, unsigned v=0):
    mod(m), val(v%m) {
        // Corpo vazio
    }
    Count & operator++() {
        if (++val >= mod )
            val = 0;
        return *this;
    }
    Count & operator--(){
        if (val-- == 0)
            val = mod - 1;
        return *this;
    }
    // Coerção de Count em unsigned
    operator unsigned () const {
        return val;
    }
    Count & operator= (unsigned v){
        val = v % mod;
        return *this;
    }
};

int main(int argc, char ** argv){
    Count c1(10,1), c2(8);  // Criação dos objectos
    while (c1) {
        std::cout << "c1 = " << ++c1 << " c2 = " << ++c2 << std::endl;
    }
    std::cout << std::endl;
    c2 = 5; // Afecta o valor ao contador.
    for (int i = 5; i; i--) {
        std::cout << "c1 = " << --c1 << " c2 = " << --c2 << std::endl;
    }
    return 0;
}