#include <botan-3/botan/version.h>
#include <iostream>

int main() {
    std::cout << "Botan version: " << Botan::version_string() << '\n';
    return 0;
}
