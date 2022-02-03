#include <iostream>
// #include "BigInt.h"
#include <cstring>

typedef unsigned char byte;     // sizeof == 8bits
typedef unsigned short word;    // sizeof == 16bits
typedef unsigned int dword;     // sizeof == 32bits


int main (int argc, char ** argv) {
    char str[] = "07006700";
    char * saux = str;
    while (*saux == '0') ++saux;
    size_t len = sizeof(str);
    std::cout << saux << std::endl;
    std::cout << str << std::endl;
    len -= saux - str;
    std::cout << len << std::endl; 
    size_t sz = len/4 + (len % 4 != 0);
    std::cout << sz << std::endl;
    
    word * v = new word[4];
    size_t nDigs = 4UL;
    --len;
    for (size_t index = 0; len; ++index, len-=nDigs) {

        if (len < 4UL)
            nDigs = len;

        word aux = 0;
        for (size_t j=nDigs;j;--j){
            aux <<= 4UL;
            std::cout << " " << len-j << " - " << saux[len-j];
            aux += tolower(saux[len-j]) - (isdigit(saux[len-j])?'0':'a'-10);
        }
        v[index] = aux;
    }
    std::cout << std::endl << v[0] << " " << v[1] << std::endl;
    return 0;
}