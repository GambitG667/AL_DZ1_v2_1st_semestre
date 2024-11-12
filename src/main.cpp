#include "Calculator.hpp"
#include <iostream>

int main(){
    std::string expresson{""};
    while(true){
        std::getline(std::cin, expresson);
        std::cout << Calculator::calculate(expresson) << std::endl;
        std::cout << "-------------\n";
    }
    return 0;
}