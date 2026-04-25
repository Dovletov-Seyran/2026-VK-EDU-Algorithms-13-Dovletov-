#include <iostream>

int GetBit(unsigned int n, int k) {
    return (n >> k) & 1;
}

int main() {
    unsigned int n = 0; 
    int k = 0;    
    std::cin >> n >> k;
    std::cout << GetBit(n, k) << std::endl;
    return 0;
}
