#include <iostream>
#include "Str1.h"

Str1 s1 = "aaaa", s2 = "bbbb", s3 = "cccc", s4 = "dddd", s5, s6;
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