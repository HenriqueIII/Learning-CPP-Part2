// << Calculo de uma potencia em tempo de compilação >>
#include <iostream>

// Template primário
template < int E >
inline int pow ( int b ) {
    int x = b*b, a = pow <E/2> (x);
    return E & 1 ? b * a : a;
}
// Especizlização para N == 0
template<>
inline int pow<0> ( int ) { return 1; }

int main(){
    std::cout << pow<5>(2);
}