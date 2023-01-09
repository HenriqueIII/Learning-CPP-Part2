#include <iostream>
#include <cstring>

class String
{
private:
// Atributos
    static const size_t DIM_MIN = 32; //Dimensão minima
    char * rep;                 // @brief Representação da **string C Type**  
    size_t sz;                  // @param Size Numero de caracteres válidos.
    size_t dim;                 // @brief Espaço reservado para representação.
// Métodos Auxiliares
    //Auxiliares dos contrutores.
    void init ( const char * st, size_t size, size_t dimension ); 
    void init ( const char * st, size_t sz )
    {
        init ( st, sz, sz );
    }
    // Auxiliar de **init()**
    void dimNormalize ( size_t dimension ); // Acertar **dim**
    // Auxiliar dos operadores afectação.
    void assign ( const char *, size_t ); // Alterar a **string**
public:
// << Construtores & destrutor >>
    String( const char * st = "" )      { init(st, strlen(st)); }
    String( const String &x )           { init(x.rep, x.sz);    }
    String(size_t c, const String & x)  { init(x.rep, x.sz, c); }
    ~String()                           { delete [] rep;        }
// << Metodos de acesso >>
    // Número de caracteres correntes.
    size_t size() const                 { return sz;            }
    // Capacidade máxima em caracteres.
    size_t capacity() const             { return dim - 1;       }
    // Acesso a **String C-style**
    const char * c_str() const          { return rep;           }
    // Leitura do caracter situado em **index**
    char readAt(size_t index) const     { return rep[index];    }
    // Escrita (afectação) de um caracter em **index**
    void writeAt(size_t index, char c)  { rep[index] = c;       }
// << Operadores Afetação >>
    String &operator= (const String &x)
    { if (this != &x) assign(x.rep, x.sz); return *this;        }
    String &operator= (const char * st)
    { assign(st, strlen(st)); return *this;                     }
// << Operador de concatenação >>
    String &operator+= (const String &s);   // **Append**
};
// Append
inline String operator+(const String &x1, const String &x2) 
{   String tmp (x1.size()+x2.size(), x1); return tmp += x2;     }
// Inserção de String em ostream
inline std::ostream &operator<<(std::ostream &o, const String &s)
{   return o << s.c_str();                                      }

void String::dimNormalize(size_t dimension){
    // duplicar **dim** em cada ampliação
    for (dim=DIM_MIN; dim < dimension || sz >= dim; dim*=2);
}

void String::init(const char * s, size_t size, size_t cap){
    sz = size;
    dimNormalize(cap+1);
    rep = new char[dim];
    memcpy(rep, s, sz+1);
}

void String::assign(const char * s, size_t size){
    if ( (sz = size) >= dim) {
        dimNormalize(sz+1);
        delete [] rep;
        rep = new char [dim];
    }
    memcpy(rep, s, sz+1);
}

String &String::operator+= (const String &x){
    size_t newSz = sz +x.sz;
    if (newSz >= dim) {
        dimNormalize(newSz+1);
        char *r = new char [dim];
        memcpy(r, rep, sz);
        delete [] rep;
        rep = r;
    }
    memcpy(rep + sz, x.rep, x.sz);
    rep[sz = newSz] = '\0';
    return *this;
}

String s1 = "aaaa", s2 = "bbbb", s3="cccc", s4="dddd", s5, s6;
// Teste de desempenho relativo das varias versões de String
int main(int argc, char ** argv) {
    clock_t tempoi = clock(); // inicio da contagem do tempo
    for (int i = 0; i < 1300; ++i) {
        s6 = "abcd";
        s5 = s1 + s2 + s3 + s4;
        s6 = s5.c_str();
        s2 = s5 + s1 + s3 + s4;
    }
    clock_t tempof = clock(); // fim da contagem do tempo
    std::cout << std::endl << s5;
    std::cout << std::endl << "size = " << s5.size();
    std::cout << std::endl << "tempo = " << (float(tempof-tempoi)/CLOCKS_PER_SEC) << " secs" << std::endl;
}

//Blibliote user 0.405s