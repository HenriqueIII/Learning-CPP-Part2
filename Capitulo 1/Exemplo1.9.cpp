#include <iostream>

class Mini {
    unsigned char v;
    // Só afecta se o valor pertencer à gama.
    void assign (int i){
        if (i < 0 || i >= 63 ){
            std::cout << "Erro na gama" << std::endl;
            exit(-1);
        }
        v = i;
    }
public:
    // Construtor que promove a coerção de int em Mini.
    Mini(int i) {   assign (i); }
    // Operador coerção Mini em int. Não explicita tipo de
    // retorno visto ser redundante.
    operator int() const {  return v;   }
    // Operador afectação com um Mini.
    Mini &operator=(const Mini &t) {    v = t.v; return *this;  }
    // Operador afectação com int
    Mini &operator=(int i) {    assign(i); return *this;    }
};

int main (int argc, char ** argv){
    Mini c1 = 2;
    Mini c2 = 62;
    Mini c3 = c2 - c1;  // c3 = 60 coerção de Mini em inteiro
                        // dado que Mini não tem sobrecarga
                        // para a operação "-"
    Mini c4 = c3;       // Não testa gama (não é necessário)
    int i = c1 + c2;    // i = 64; coerção de Mini em inteiro
    c1 = c2 + 2 * c1;   // Erro - coerção de inteiro em Mini
    c2 = c1 - i;        // Erro na gama - coerção inteiro em Mini.
    c3 = c2;            // Não testa gama (não é necessário)

    return 0;
}