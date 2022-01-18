#include <iostream>
#include <string.h>

class String {
// <<Atributos>>
    static const size_t DIM_MIN = 32;   // Dimensão mínima
    char * rep;         // Representação da string C-type
    size_t sz;          // Size. Número de caracteres válidos
    size_t dim; // Espaço reservado para a representação.
// <<Métodos auxiliares>>
    // Auxiliares de construtores
    void init(const char *st, size_t size, size_t dim);
    void init(const char *st, size_t size) { init (st, size, size); }
    // Auxiliar de init()
    void dimNormalize(size_t dimension);    // Acertar dim.
    // Auxiliar de operadores de afectação
    void assign(const char *, size_t);  // Alterar a String
public:
// <<Construtores & Destrutor>>
    String(const char * st = "")        { init(st, strlen(st)); }
    String(const String &x)             { init(x.rep, x.sz);    }
    String(size_t c, const String &x)   { init(x.rep, x.sz, c); }
    ~String()                           { delete[] rep;         }
// <<Metodos de Acesso>>
    // Numero de caracteres correntes
    size_t size() const                 { return sz;            }
    // Capacidade máxima em caracteres
    size_t capacity() const             { return dim - 1;       }
    // Acesso a String C-Style
    const char * c_str() const          { return rep;           }
    // Leitura do caracter situado em index
    char readAt(size_t index) const     { return rep[index];    }
    // Escrita (afectação) de um caracter em index
    void writeAt(size_t index, char c)  { rep[index] = c;       }
// <<Operadores afectação>>
    String &operator= (const String &x) {
        if (this != &x)
            assign(x.rep, x.sz);
        return *this;
    }
    String &operator= (const char * st) {
        assign(st, strlen(st));
        return *this;
    }
// <<Operador de concatenação>>
    String &operator+= (const String &s);   // Append.
};
// Append (concatenação); função global simétrica.
inline String operator+(const String &x1, const String &x2) {
    String tmp (x1.size() + x2.size(), x1);
    return tmp += x2;
}
// Inserção de String em ostream.
inline std::ostream &operator<< (std::ostream &o, const String &s) {
    return o << s.c_str();
}

void String::dimNormalize(size_t dimension) {
    // Duplicar dim em cada ampliação
    for (dim = DIM_MIN; dim < dimension || sz >= dim; dim *= 2);
}

void String::init(const char * s, size_t size, size_t cap) {
    sz = size;
    dimNormalize(cap + 1);
    rep = new char[dim];
    memcpy(rep, s, sz+1);
}

void String::assign(const char *s, size_t size){
    if ( (sz = size) >= dim ) {
        dimNormalize(sz +1);
        delete[] rep;
        rep = new char[dim];
    }
    memcpy(rep, s, sz + 1);
}

String & String::operator+= ( const String & x) {
    size_t newSz = sz + x.sz;
    if ( newSz >= dim ) {
        dimNormalize( newSz + 1 );
        char *r = new char[dim];
        memcpy(r, rep, sz);
        delete [] rep;
        rep = r;
    }
    memcpy(rep + sz, x.rep, x.sz);
    rep[sz = newSz] = '\0';
    return * this;
}

//#include <string>
//typedef std::string String;

String s1 = "aaaa", s2 = "bbbb", s3 = "cccc", s4 = "dddd", s5, s6;
// Teste de desempenho relativo das várias versões de String
int main(int argc, char ** argv){
    clock_t tempoi = clock();   // Inicio da contagem do tempo
    for (int i = 0; i < 1300; ++i ) {
        s6 = "abcd";
        s5 = s1 + s2 + s3 + s4;
        s6 = s5.c_str();
        s2 = s5 + s1 + s3 + s4;
    }
    clock_t tempof = clock();   // Fim da contagem do tempo
    std::cout << std::endl << s5;
    std::cout << std::endl << "size = " << s5.size();
    std::cout << std::endl << "tempo = " << (float(tempof-tempoi) / CLOCKS_PER_SEC) << " secs" << std::endl;
}

//String C++ 0.006055 secs
//String definida 0.016867 secs
