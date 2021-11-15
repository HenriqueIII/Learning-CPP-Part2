#include <iostream>
#include "String.h"         // Definição de uma classe string, cuja
                            // interface seja idêntica à definida.
// Armazena uma string e um valor inteiro
struct Pair {
    String key;             // Palavra chave de pesquisa (Índice)
    unsigned value;         // Numero de ocorrencias da palavra.
    Pair () { value = 0; }  //Iniciar por omissão
};
// Associa pares constituidos por uma string e um inteiro
class Assoc {
    Pair *array;    // Apontador para a tabela.
    int max;        // Dimensão reservada para a tabela.
    int free;       // Indice livre ou número de elementos contidos.
public:
    class Const_Iterator {
        friend class Assoc;
        Pair *current;      // Posição corrente
        // Construtor usado pela classe Assoc.
        Const_Iterator(Pair *p) { current = p; }
    public:
        Const_Iterator() { } // Construtor por omissão
        const Pair &operator*() { return *current; }
        const Pair *operator->() { return current; }
        Const_Iterator &operator++ () {
            ++current; return *this;
        }
        Const_Iterator &operator++(int) {
            Const_Iterator aux = *this; ++current; return aux;
        }
        bool operator==(const Const_Iterator &i) {
            return current == i.current;
        }
        bool operator!=(const Const_Iterator &i) {
            return current != i.current;
        }
    };
    Assoc(int dim); // Construtor com dimensão dim.
    ~Assoc() { delete [] array; }
    // Operador indexação (indice é o membro chave do par).
    unsigned &operator[]( const String& );  // Retorna por referência
    void print_all();   // Imprimir todos os pares
    Const_Iterator begin() const {
        return array;
    }
    Const_Iterator end() const {
        return array+free;
    }
};

Assoc::Assoc(int s) {
    max = (s < 16) ? 16 : s;    // Minima dimensão 16.
    free = 0;                   // Elementos ocupados.
    array = new Pair[max];      // Reserva de espaço para a tabela.
}

unsigned &Assoc::operator[]( const String &k) {
    Pair *pp;
    // Pesquisar no array.
    for (pp = array; pp != (array+free); ++pp)
        if (k == pp->key )    // Comparar dois objectos String
            return pp->value;   // Retornar ocorrências (referência).
    // Inserir no array um novo Pair. Se necessário reservar
    // espaço e copiar para o fim a palavra indice
    if (free == max) {  // Overflow: duplica espaço reservado.
        Pair * nvec = new Pair[ max*2 ];
        // Copiar Pairs já existentes para o novo espaço.
        for (int i = 0; i < max; ++i) nvec[i] = array[i];
        // Devolver espaço anteriormente reservado.
        delete [] array;
        // Situar array no novo espaço
        array = nvec; max *= 2;
    }
    pp = &array[free++]; pp->key = k;
    return pp->value;   // Retornar ocorrencias por referencia. 
}

int main (int argc, char ** argv) {
    String word;        // Buffer para as palavras recolhidas
    Assoc table(256);   // Tabela associativa.
    // Conta ocorrências de cada palavra na entrada.
    while(std::cin >> word) ++table[word];
    // Listar todas as ocorrências no console output
    Assoc::Const_Iterator i;
    for (i = table.begin(); i != table.end(); ++i){
        std::cout << i->key << " : " << i->value << std::endl;
    }
    // Listar no console output os pares cujas palavras
    // tenham ocorrências superiores ou iguais à média
    unsigned avg = 0, count = 0;
    for (i = table.begin(); i != table.end(); ++count, ++i)
        avg+=i->value;
    avg /= count;
    std::cout << "Palavras com contagem superior à média("<< avg <<"):" << std::endl;
    for (i = table.begin(); i != table.end(); ++i)
        if (i->value > avg)
            std::cout << i->key << " : " << i->value << std::endl;
    return 0;
}