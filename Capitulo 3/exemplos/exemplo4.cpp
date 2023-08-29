#include <iostream>

template <class T> struct AvgTraits {
    // Tipo do resultado da média, caso T não tome o tipo int
    typedef T AvgRes;
};

template <> struct AvgTraits<int> {
    // Tipo do resultado da média, caso T tome o tipo int
    typedef float AvgRes;   // Especialização para int
};

// Template de funções sum, auxiliares de avg
template<class T> T sum ( T *array, int dim) {
    T res = 0;
    for ( ; dim; --dim, ++array) res += *array;
    return res;
}

// Template de funções avg, para calculo da média
template<class T>
typename AvgTraits<T>::AvgRes avg( T *array, int dim) {
    typename AvgTraits<T>::AvgRes res = sum(array, dim);
    return res / dim;
}

int main(int argc, char ** argv) {
    const int DIM = 10;
    int array[DIM] = {10, 10, 10, 9, 9, 10, 10, 10, 10, 10};
    for (int i = 0; i < DIM; ++i){
        std::cout << array[i] << std::endl;
    }
    typename AvgTraits<int>::AvgRes res =  avg<int>(array, DIM);
    std::cout << "media = " << res << std::endl;
    return 0;
}