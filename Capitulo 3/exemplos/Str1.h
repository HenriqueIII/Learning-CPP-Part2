// Definição da classe str1
#ifndef _STR1_H
#define _STR1_H

#include <iostream>
#include <cstring>

class Str1
{
private:
// Atributos
    static const size_t DIM_MIN = 32; //Dimensão minima
    char * rep;                 // Representação da **String C Type**  
    size_t sz;                  // Numero de caracteres válidos.
    size_t dim;                 //Espaço reservado para representação.
// Métodos Auxiliares
    //Auxiliares dos contrutores.
    void init ( const char * st, size_t size, size_t dimension = DIM_MIN ); 
    // Auxiliar de **init()**
    void dimNormalize ( size_t dimension ); // Acertar **dim**
    // Auxiliar dos operadores afectação.
    void assign ( const char *, size_t ); // Alterar a **Str1**
    void append ( const char *s, size_t size );
public:
// << Construtores & destrutor >>
    Str1( const char *s, size_t size, size_t dim = DIM_MIN) { init(s, size, dim);   }
    Str1( const char * st = "" )      { init(st, strlen(st)); }
    Str1( const Str1 &x )           { init(x.rep, x.sz);    }
    Str1(size_t c, const Str1 & x)  { init(x.rep, x.sz, c); }

    ~Str1()                           { delete [] rep;        }
// << Metodos de acesso >>
    // Número de caracteres correntes.
    size_t size() const                 { return sz;            }
    // Capacidade máxima em caracteres.
    size_t capacity() const             { return dim - 1;       }
    // Acesso a **Str1 C-style**
    const char * c_str() const          { return rep;           }
    // Leitura do caracter situado em **index**
    char readAt(size_t index) const     { return rep[index];    }
    // Escrita (afectação) de um caracter em **index**
    void writeAt(size_t index, char c)  { rep[index] = c;       }
// << Operadores Afetação >>
    Str1 &operator= (const Str1 &x)
    { if (this != &x) assign(x.rep, x.sz); return *this;        }
    Str1 &operator= (const char * st)
    { assign(st, strlen(st)); return *this;                     }
// << Operador de concatenação >>
    Str1 &operator+= (const Str1 &s);   // **Append**
    Str1 &operator+= (const char *s);
};
// Append
inline Str1 operator+(const Str1 &x1, const Str1 &x2) 
{   Str1 tmp (x1.size()+x2.size(), x1); return tmp += x2;     }
// Inserção de Str1 em ostream
inline std::ostream &operator<<(std::ostream &o, const Str1 &s)
{   return o << s.c_str();                                      }
inline Str1 &Str1::operator+= (const Str1 &x) 
	{  append(x.rep, x.sz); return *this; }
inline Str1 &Str1::operator+= (const char *s) 
	{  append(s, strlen(s)); return *this; }

#endif
