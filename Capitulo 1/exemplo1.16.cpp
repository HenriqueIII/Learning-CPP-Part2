#include <iostream>
#include <fstream>
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
        Const_Iterator operator++(int) {
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

struct Print {
    enum Mode { CAB = 0x01, WAIT = 0x02};
    unsigned count;     // Numero de elementos escritos
    unsigned val;       // Valor a comparar
    std::ostream &out;  // ostream para escrita
    unsigned mode;      // Formato da listagem
    // Se na construção for omitido:
    //      3º Parametro - lista todas as palavras
    //      2º Parametro - escreve cabeçalho e suspende a escrita
    //      1º Parametro - lista no console output
    Print (std::ostream &o = std::cout, unsigned m = CAB|WAIT, unsigned v = 0):
    mode(m), out (o), count(v), val(v) { }
    // Sobrecarga do operador chamada a função
    void operator() (const Pair &p) {
        const unsigned NUM_OF_LINES = 22;
        if (p.value >= val){
            // No inicio de cada pagina esvre o cabeçalho
            if ((mode&CAB) && count % NUM_OF_LINES == 0)
                out << "Ocorrencias Palavra " << std::endl;
            out << std::setw(6) << p.value << std::setw(6) << ' ' << p.key << std::endl;
            ++count;
            // Suspende a escrita no fim de cada pagina.
            if ((mode&WAIT) && count % NUM_OF_LINES == 0)
                std::cin.get();
        }
    }
};

Print print (Assoc::Const_Iterator first, Assoc::Const_Iterator last, Print p = Print()) {
    while (first != last)
        p(*first++);
    return p;
}

void error (const char *str) {
    std::cerr << str << std::endl;
    exit(-1);
}

unsigned avg(const Assoc &tb) {
    unsigned sum = 0, count = 0;
    for (Assoc::Const_Iterator i = tb.begin(); i != tb.end(); ++i, ++count)
        sum += i->value;
    return sum / count;
}

int main (int argc, char ** argv) {
    // Testar o número de argumentos da linha de comando
    if (argc != 2 && argc != 3)
        error("Número de argumentos da linhas de comando incorreto");
    // Criar o objecto istream e associa-lo ao ficheiro cujo
    // pathname é o valor de argv[1]
    std::ifstream fin(argv[1]);
    if (!fin)
        error("Erro ao abrir o ficheiro input");
    String word;        // Buffer para as palavras recolhidas
    Assoc table(256);   // Tabela associativa.
    // Conta ocorrências de cada palavra na entrada.
    while(fin >> word) ++table[word];
    // Listar todas as ocorrências no console output
    print (table.begin(), table.end());
    // Listar num ficheiro de texto os pares cujas palavras
    // tenham ocorrências superior ou iguais à média.
    if (argc == 3) {
        std::ofstream fout(argv[2]);
        if (!fout)  // Testar se a abertura foi bem sucedida
            error("Erro ao abrir o ficheiro de output.");
        Print p = print(table.begin(), table.end(), Print(fout, 0, avg(table)));
        std::cout << "Ocorreram " << p.count << " palavras com " << p.val << " ocorrencias." << std::endl;
    }
    return 0;
}