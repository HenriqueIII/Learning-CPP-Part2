#include <iostream>
#include <cstring>

class StrTmp;   // Declaração Prévia
class CRef;     // Tipo retornado na indexação

class String {
// Representação
    class SRep {
    public:
    // Atributos
        static const size_t DIM_MIN = 32; //Dimensão minima
        char * rep;                 // @brief Representação da **string C Type**  
        int count;                  // contador de referências à representação
        size_t sz;                  // @param Size Numero de caracteres válidos.
        size_t dim;                 // @brief Espaço reservado para representação.
    // << Construtor & Destrutor >>
        SRep(const char * s, size_t size, size_t cap = DIM_MIN - 1);
        ~SRep() { delete [] rep; }
    // << Métodos Auxiliares >>
        // Normalizar dimensão
        void dimNormalize ( size_t dimension );
        // Afectar a representação
        void assign ( const char * s, size_t size);
        // Acrescentar texto no final do corrente.
        void append(const SRep & r);
    // << Gestão do contador de referências >>
        void incRef() { ++count; }
        void decRef() { if (--count == 0) delete this; }
    private:
        // Não permitir a construção por cópia
        SRep( const SRep & ) { }
        // Não permitir a afetação
        void operator= ( const SRep & ) { }
    };
// << Classe auxiliar nested >>
    class SRepPtr {
    // << Atributos >>
        SRep * p;       // Apontador para representação
    public:
    // << Construtores e Destrutor >>
        SRepPtr(SRep * sr) : p(sr)               { p->incRef();  }
        SRepPtr(const SRepPtr &srp) : p(srp.p)  { p->incRef();  }
        ~SRepPtr()                              { p->decRef();  }
    // << Operadores afectação >>
        void operator=(SRep *s)
        { s->incRef(); p->decRef(); p=s; }
        void operator=(const SRepPtr &srp)
        { *this = srp.p; }
    // << Operadores de desreferência >>
        SRep &operator*() const                 { return *p;    }
        SRep *operator->() const                { return p;     }
    };
// << Atributos de String >>
    SRepPtr srep; // Representação da String (Único atributo)
public:
// << Construtores & destrutor >>
    String( const char * s = "" ) : srep (new SRep(s,strlen(s))) { };
    String(size_t cap, const String & x) : srep (new SRep (x.c_str(), x.size(), cap)) { }
    String(const StrTmp &st);
// << Metodos de acesso >>
    // Número de caracteres correntes.
    size_t size() const                 { return srep->sz;      }
    // Capacidade máxima em caracteres.
    size_t capacity() const             { return srep->dim - 1; }
    // Acesso a **String C-style**
    const char * c_str() const          { return srep->rep;     }
// << Operadores Afetação >>
    String &operator= (const char * st);
// << Operador de concatenação >>
    String &operator+= (const String &s);   // **Append**
// << Acesso a caracteres >>
    // Leitura do caracter situado em **index**
    char readAt(size_t index) const     { return srep->rep[index];}
    // Escrita (afectação) de um caracter em **index**
    void writeAt(size_t index, char c);
// << Operador indexação >>
    char operator[] (size_t ind) const  {return readAt(ind);    }
    CRef operator[] (size_t ind);
};

// Afecta directamente o atributo **dim**.
void String::SRep::dimNormalize(size_t dimension){
    // Duplicar **dim** quanto o necessário.
    for (dim=DIM_MIN; dim < dimension || sz >= dim; dim*=2);
}

class StrTmp {
    friend class String;
    String str; // Atributo único
// << Construtor >>
    StrTmp(size_t dim, const String &x) : str(dim, x) { }
public:
// << Concatenação >>
    StrTmp &operator+(const String &x) {
        str += x; return *this;
    }
    friend StrTmp operator+(const String &a, const String &b);
};

class CRef {
    String &rs;
    const size_t index;
public:
    CRef(String &s, size_t i) : rs(s), index(i) { }
    // Invocado para escrita
    CRef &operator=(char c) {
        rs.writeAt(index, c); 
        return *this;
    }
    CRef &operator=(const CRef &r) {
        return *this = char(r);
    }
    // Invocado para leitura
    operator char() const {
        return rs.readAt(index);
    }
};

StrTmp operator+(const String &a, const String &b){
        StrTmp tmp (a.size() + b.size(), a);
        return tmp + b;
    }

String::SRep::SRep(const char * s, size_t size, size_t cap) {
    count = 0;
    sz = size;
    dimNormalize(cap +1); // Afecta directamente dim
    rep = new char [dim];
    memcpy(rep, s, sz+1);
}

inline String::String(const StrTmp &st) : srep(st.str.srep) { }

void String::SRep::assign(const char * s, size_t size) {
    sz = size;
    if ( sz >= dim) {
        dimNormalize(sz + 1);
        delete [] rep;
        rep = new char [dim];
    }
    memcpy(rep, s, sz+1);
}

String &String::operator= (const char * s) {
    if (srep->count > 1)  // Separar e criar uma cópia.
        srep = new SRep(s, strlen(s));
    else
        srep->assign(s, strlen(s));
    return *this;
}

void String::SRep::append(const SRep &x) {
    size_t newSz = this->sz + x.sz;
    if (newSz >= this->dim)   {
        dimNormalize(newSz + 1);
        char * r = new char [dim];
        memcpy(r, this->rep, sz);
        delete [] rep;
        rep = r;
    }
    memcpy(this->rep+this->sz, x.rep, x.sz);
    rep[sz=newSz] = '\0';
}

// Inserção de String em ostream
inline std::ostream &operator<<(std::ostream &o, const String &s)
{   return o << s.c_str();                                      }

String &String::operator+= (const String &s){
    if(srep->count > 1) 
        srep = new SRep(c_str(), size(), size()+s.size());
    srep->append(*s.srep);
    return *this;
}

void String::writeAt(size_t index, char c) {
    if(srep->count > 1){
        srep->decRef();
        srep = new SRep(c_str(), size());
        srep->incRef();
    }
    srep->rep[index]=c;
}

inline CRef String::operator[] (size_t i) {
    return CRef(*this, i);
}

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
//Blibliote user 0.405s