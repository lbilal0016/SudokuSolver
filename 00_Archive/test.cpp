#include <iostream>

int main(){

    #if __cplusplus == 202302L
    std::cout << "C++23" << std::endl;
    #endif
    return 0;
}