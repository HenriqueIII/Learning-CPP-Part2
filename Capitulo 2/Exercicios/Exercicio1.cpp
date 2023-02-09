#include <iostream>
#include "Fraction.h"

int main(int argc, char ** argv) {
    Fraction seno(0L),coseno(0L),arcseno(0L);
    seno = sin(30, 10);
    coseno = cos(60, 10);
    arcseno = arcsin(seno);
    seno.showDec();
    std::cout << std::endl;
    coseno.showDec();
    std::cout << std::endl;
    arcseno.showDec();
    std::cout <<std::endl;
    degree(arcseno).showDec();
    std::cout << std::endl;
    return 0;
}