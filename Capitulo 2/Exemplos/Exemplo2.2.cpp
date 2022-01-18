#include <iostream>
#include <cstring>

class String {
//  <<Representação>>
    class SRep {
    public:
    //  <<Atributos>>
        static const size_t DIM_MIN = 32;   // Dimensão Minima
        char *rep;  // Representação da string C type
        int count;  // Contador de referências à representação
        size_t sz;  // Número de caracteres úteis da cadeia
        size_t dim; // Dimensão do espaço reservado
    // <<Construtor & Destrutor>>
        SRep (const char * s, size_t size, size_t cap = DIM_MIN - 1);
        ~SRep () { delete [] rep; }
    // <<Métodos auxiliares>>
        // Normalizar a dimensão
        void dimNormalize(size_t dimension);
        // Afectar a representação
        void assign(const char * s, size_t size);
        // Acrescentar texto no final do corrente
        void append(const SRep & r);
    // <<Gestão do contador de referências>>
        void incRef() { ++count; }
        void decRef() { if (--count == 0) delete this; }
    private:
        // Não permitir a construção por cópia
        SRep( const SRep & ) {}
        // Não permitir a afectação
        void operator=( const SRep &) {}
    };
// <<Atributos>>
    SRep * srep;    // Representação da String (único atributo)
public:
// <<Construtores & Destrutor>>
    String(const String & x);
    String(const char * s="");
    String(size_t dimension, const String & x);
    ~String() { srep->decRef(); }
// <<Métodos de acesso>>
    size_t size() const         { return srep->sz;      }
    size_t capacity() const     { return srep->dim-1;   }
    const char * c_str() const  { return srep->rep;     }
// <<Operadores de afectação>>
    String &operator= ( const String &);
    String &operator= ( const char *);
// <<Operadores de concatenação>>
    String &operator+= ( const String &);   // Append
// <<Acesso a caracteres>>
    char readAt (size_t index) const { return srep->rep[index];}
    void writeAt (size_t index, char c);
};
inline String operator+(const String &x1, const String &x2){
    String tmp(x1.size()+x2.size(), x1);
    return tmp += x2;
}
inline std::ostream &operator<< ( std::ostream &o, const String &x) {
    return o << x.c_str();
}

//Afecta directamente o atributo dim
void String::SRep::dimNormalize( size_t dimension ) {
    // Duplicar dim quanto o necessário
    for (dim = DIM_MIN; dim <= dimension || sz >= dim; dim*=2);
}

String::SRep::SRep ( const char * s, size_t size, size_t cap ) {
    count = 0;
    sz = size;
    dimNormalize (cap + 1); // Afecta directamente dim.
    rep = new char[dim];
    memcpy(rep, s, sz + 1);
}

inline String::String (const String & x): srep(x.srep) {
    srep->incRef();
}

String::String(const char *s) :
    srep ( new SRep(s,strlen(s)))
{
    srep->incRef();
}
String::String(size_t cap, const String &x) :
    srep ( new SRep (x.c_str(), x.size(), cap))
{
    srep->incRef();
}

String &String::operator= (const String &x) {
    x.srep->incRef();
    srep->decRef();
    srep = x.srep;
    return *this;
}

void String::SRep::assign (const char * s, size_t size) {
    sz = size;
    if (sz >= dim) {
        dimNormalize( sz + 1 );
        delete [] rep;
        rep = new char [dim];
    }
    memcpy(rep, s, sz+1);
}

String &String::operator= (const char * s) {
    if (srep->count > 1) {  // Separar e criar uma cópia
        srep->decRef();
        srep = new SRep(s, strlen(s));
        srep->incRef();
    }
    else
        srep->assign(s, strlen(s));
    return *this;
}

void String::SRep::append(const SRep & x) {
    size_t newSz = sz + x.sz;
    if (newSz >= dim) {
        dimNormalize(newSz +1);
        char * r = new char[dim];
        memcpy(r, rep, sz);
        delete[] rep;
        rep = r;
    }
    memcpy(rep + sz, x.rep, x.sz);
    rep[sz = newSz] = '\0';
}

String &String::operator+= (const String &s) {
    if (srep->count > 1){
        srep->decRef();
        srep = new SRep(c_str(), size(), size()+s.size());
        srep->incRef();
    }
    srep->append(*s.srep);
    return *this;
}
void String::writeAt(size_t index, char c) {
    if (srep->count > 1) {
        srep->decRef();
        srep = new SRep(c_str(), size());
        srep->incRef();
    }
    srep->rep[index] = c;
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

// String c++: 0.005962
// String declarada: 0.00906