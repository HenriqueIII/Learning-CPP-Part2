#include <iostream>
// #include "BigInt.h"
#include <cstring>

typedef unsigned char byte;     // sizeof == 8bits
typedef unsigned short word;    // sizeof == 16bits
typedef unsigned int dword;     // sizeof == 32bits


int main (int argc, char ** argv) {
    char str[] = "00C66700";
    char * saux = str;
    while (*saux == '0') ++saux;
    size_t len = sizeof(str);
    std::cout << saux << std::endl;
    std::cout << str << std::endl;
    len -= saux - str;
    std::cout << len << std::endl; 
    return 0;
}