#include "BigInt.h"

int main ()
{
    BigInt bigInt("2"), bigInt2 = 100;
    std::cout << bigInt << "\n"
    << bigInt + bigInt2 << "\n"
    << bigInt - bigInt2 << "\n"
    << bigInt * bigInt2 << "\n"
    << bigInt2 / bigInt << "\n";

    return 0;
}