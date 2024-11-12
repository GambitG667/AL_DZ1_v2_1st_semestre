#include "Token.hpp"


Token::Token(TYPE t){
    type = t;
    value = 0;
    var = "";
}

Token::Token(float number){
    type = NUMB;
    value = number;
    var = "";
}

Token::Token(std::string str){
    type = VAR;
    value = 0;
    var = str;
}