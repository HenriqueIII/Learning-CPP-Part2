#include <iostream>
#include <cstring>
#include <assert.h>
#include <iomanip>
#include <ctime>
#include "BigInt.h"


int main(int argc, char ** argv){
    BigInt n1 = 99999999L, n2, n3=6871209L, n4=724695L, n5;
    std::cout << n1 << '-' << n3 << '-' << n4 << '-' << n5 << std::endl;
    clock_t tempoi = clock();
    for (int i=0;i<10; ++i) {
        n3 = n1 * n3;
        n3 += BigInt("-123456780", BigInt::DEC);
        n5 = n3/n4;
        n5 = n5 * n4 + n3 % n4 - n3;
        n3 -= -123456780L;
        if (n5 != 0L) std::cout << "ERRO" << std::endl;
    }
    clock_t tempof = clock();
    std::cout << "\nn3=" << n3 << "\nn5=" << n5 << std::endl;
    std::cout << "Tempo = " << float(tempof-tempoi)/CLOCKS_PER_SEC << "secs" << std::endl;
    BigInt bHex("f7cd4", BigInt::HEX);
	BigInt bDec("765432", BigInt::DEC);
	BigInt bTxt ("Texto a cifrar", BigInt::TEXT);
	std::cout << std::hex << bHex << std::endl; // Valor em hexadecimal.
	std::cout << std::dec << bHex << std::endl; // Valor em decimal.
	std::cout << std::dec << bDec << std::endl; // Valor em decimal.
	std::cout << std::hex << bDec << std::endl; // Valor em hexadecimal.
	std::cout << txt << bTxt << std::endl; // Texto.
	std::cout << std::hex << bTxt << std::endl; // Valor em hexadecimal.
    return 0;
}

/* Output 
99999999-6871209-724695-0

n3=687120831287913092043967545493442953872684553464295388175454923092044043128791006871209
n5=0
Tempo = 0.001secs
000f 7cd4
1014996
765432
000b adf8
Texto a cifrar
7261 7266 6963 2061 206f 7478 6554
*/