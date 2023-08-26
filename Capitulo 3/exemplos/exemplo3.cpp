#include <iostream>
#include <cassert>

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
            if (a[j] > a[j-1])
                swap (a[j], a[j-1]);
}

template<class T>
std::ostream &operator<< (std::ostream &o, Array<T> &a) {
    if ( a.size() > 0) {
        o << a[0];
        for (unsigned i=1; i < a.size(); ++i)
            o << ',' << a[i];
    }
}

int main(int argc, char ** argv) {
    return 0;
}