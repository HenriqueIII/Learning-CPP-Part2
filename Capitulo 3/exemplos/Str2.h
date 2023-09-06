#ifndef _STR2_H
#define _STR2_H

#include "Str1.h"
#include <iostream>
#include <cstring>

class Str2 {
// << Representação >>
    class StrBody : public Str1 { // Body deriva de Str1
    public:
        int count; // contador de referências à representação
    // << Construtores >>
        StrBody(const char * s, size_t size, size_t dimension)
            : Str1 (s, size, dimension), count(0) {}
        StrBody(const char * s, size_t size)
            : Str1 (s, size), count (0) {}
    // << Gestão do contador de referências >>
        void incRef()   { ++count;  }
        void decRef()   { if (--count == 0) delete this; }
    private:
        StrBody(const StrBody &);   // Não permitir copia
    };

// << Atributos >>
    StrBody * body; // a representação da string
public:
// << Construtores & Destructor >>
    Str2(const char *s="") {
        body = new StrBody(s, strlen(s)); 
        body->incRef(); 
    }

    Str2(const Str2 &x) {
        body = x.body;
        body -> incRef();
    }

    ~Str2() {
        body->decRef();
    }
// << Metodos de acesso >>
    size_t size() const         { return body->size();      }
    size_t capacity() const     { return body->capacity();  }
    const char *c_str() const   { return body->c_str();     }
// << Operadores de afetação >>
    Str2 &operator= (const Str2&);
    Str2 &operator= (const char*);
// << Operadores de concatenação >>
    Str2 &operator+= (const Str2 &);
    Str2 &operator+= (const char *);
// << Acesso a caracteres >>
    char readAt(size_t idx) const { return body->readAt(idx);}
    char writeAt(size_t idx, char c);
};

inline std::ostream &operator<< (std::ostream &o, const Str2 & s){
    return o << s.c_str();
}
inline Str2 & Str2::operator= (const Str2 &s){
    s.body->incRef();
    body->decRef();
    body = s.body;
    return *this;
}
inline Str2 & Str2::operator= (const char * s) {
    if (body->count > 1){   // Separar e criar uma cópia
        body->decRef();
        body = new StrBody(s, strlen(s));
        body->incRef(); 
    }else
        *((Str1*)body)=s;
    return *this;
}

inline Str2 & Str2::operator+=(const Str2 & s){
    if (body->count > 1){
        body->decRef();
        body = new StrBody(c_str(), size(), size()+s.size());
        body -> incRef();
    }
    *((Str1 *)body) += *((Str1 *)s.body);
    return *this;
}

inline Str2 & Str2::operator+=(const char * s) {
    if (body->count > 1) {
        body->decRef();
        body = new StrBody(c_str(), size(), size()+strlen(s));
        body->incRef(); 
    }
    *((Str1 *)body) += s;
    return *this;
}


#endif