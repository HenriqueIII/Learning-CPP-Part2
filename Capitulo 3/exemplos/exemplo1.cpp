#include <iostream>
#include <cassert>

class Stack {
public:
    typedef char T;                 // O tipo de cada elemento
    static const unsigned DIM = 32; // Dimensão de stack
private:
    T data[DIM];        // contentor estatico que suporta o stack
    unsigned size;      // Numero de objectos contidos
public:
    Stack()                 { size = 0;                               }
    bool empty() const      { return !size;                           }
    // retorna a cópia do objecto situado no topo do stack
    T &top()                { return data[size-1];                    }
    // insere objecto no topo do stack
    void push(const T &p)   { assert (DIM != size); data[size++] = p; }
    // Remove o objecto do topo do stack.
    void pop()              {  assert (size>0); --size;               }
    void print () const     { for(int i=0; i<size; ++i) std::cout << data[i];}

};

int main(int argc, char ** argv) {
    Stack stk1;
    stk1.push('a');
    stk1.print();
    return 0;
}