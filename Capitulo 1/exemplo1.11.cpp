#include <iostream>

class Spy {
    int k;
public:
    Spy(int x){
        std::cout << "\t- chamada ao construtor com int\n";
        k = x;
    }
    ~Spy(){
        std::cout << "\t- chamada ao destrutor\n";
    }
    Spy &operator=(const Spy &x){
        std::cout << "\t- chamada ao operador afetação\n";
        k = x.k;
        return *this;
    }
    Spy (const Spy &w) {
        std::cout << "\t- chamada ao construtor por copia\n";
        k = w.k;
    }
    Spy &operator++ () {
        return *this;
    }
    Spy operator++ (int) {
        return (Spy(k++));
    }
};

Spy f(Spy x) { return x; }

int main(int argc, char ** argv) {
    std::cout << "// Criar e inicializar Spy n1 = 3.\n";
    Spy n1 = 3;
    std::cout << "// Criar por copia um Spy n2 = n1.\n";
    Spy n2 = n1;
    std::cout << "// Afectar n1 com 4.\n";
    n1 = 4;
    std::cout << "// Afectar n2 com ++n1.\n";
    n2 = ++n1;
    std::cout << "// Afectar n2 com n1++.\n";
    n2 = n1++;
    std::cout << "// Afectar n2 com f(n1++).\n";
    n2 = f(n1++);
    std::cout << "// Ao terminar main().\n";
    return 0;
}