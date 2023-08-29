// << Calculo do factorial em tempo de compilação >>
#include <iostream>

template <unsigned N> // Template primario
inline unsigned fact() { return N * fact<N-1>(); }
template<> // Especialização para N == 0
inline unsigned fact<0>() { return 1; }

int main(int argc, char** argv) {
    std::cout << fact<4>() << std::endl;
    return 0;
}