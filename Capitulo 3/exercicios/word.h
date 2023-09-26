#ifndef WORD_H
#define WORD_H

#include <iostream>
#include <cassert>
#include <cstring>

template<class T>
void swap (T &a, T &b)
{ T aux = a; a = b; b = aux;}

// Definição do template de classes array
template<class T>class Array {
    T *v;       // Apontador para a representação
    unsigned dim;   // Capacidade da representação
    unsigned sz;    // Numero de elementos contidos
    Array(const Array &);   //Impossibilitar construçao por copia
    void operator= (const Array&);  // Impossibilitar afetação
public:
    explicit Array(unsigned d); // Iniciando com dim = d
    ~Array()                    {delete [] v;                }
    T &operator[] (unsigned n)  {assert (n<dim); return v[n];}
    const T &operator[] (unsigned n) const 
    { assert (n<dim);return v[n]; }
    unsigned size() const       {return sz;                  }
    void operator << (const T &e)   // Inserir no fim do array
    { assert (size() < dim); v[sz++] = e;}
};

template <class T>
Array<T>::Array(unsigned d) {
    v = new T [dim = d]; sz = 0;
}

template<class T> void sort(Array<T> &a) {
    for (unsigned i=a.size()-1; i > 0; --i)
        for (unsigned j = 1; j<=i; ++j)
            if (a[j] < a[j-1])
                swap (a[j], a[j-1]);
}

void sort(Array<char*> &v) {
    for (unsigned i = v.size()-1; i > 0; --i)
        for (unsigned j = 1; j<= i; ++j)
            if ( strcmp ( v[j], v[j-1] ) < 0 )
                swap(v[j], v[j-1]);
}

// Classe para representar palavras de texto (mini string)

class Word{
    char * ptw;
public:
    Word(char *pt = (char*)" ")    // Construtor com string C-Style
    { ptw = new char[strlen(pt)+1]; strcpy(ptw, pt); }
    Word(const Word &w)     // Construtor por cópia
    { ptw = new char[strlen(w.ptw)+1]; strcpy(ptw, w.ptw); }
    // Destrutor
    ~Word() { delete [] ptw; }
    // Operadores afetação
    void operator=(const char *pt) {
        delete [] ptw;
        ptw = new char[strlen(pt)+1]; 
        strcpy(ptw, pt);
    }
    void operator= (const Word &w) {
        *this = w.ptw;
    }
    // Operador de coerção para C String style
    operator const char* () const { return ptw; }
    // Operador de comparação
    int operator<(const char *p) const
    { return strcmp(ptw, p) < 0; }
    // retornar string C Style
    const char * getStr() const {
        return ptw;
    }
};

template<class T>
std::ostream &operator<< (std::ostream &o, Array<T> &a) {
    if ( a.size() > 0) {
        o << a[0];
        for (unsigned i=1; i < a.size(); ++i)
            o << ',' << a[i];
    }
    return o;
}

#endif