#include "Token.hpp"
#include <cstddef>
#include <vector>


class Calculator{
private:


public:
    static Token str_to_token(std::string& str, std::size_t& position);
    static std::vector<Token> tokenisation(std::string expression);
    static float calculate(std::string expression);
};