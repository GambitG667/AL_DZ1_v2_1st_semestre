#include "Calculator.hpp"
#include <cmath>
#include <cstddef>
#include <map>
#include <regex>
#include <string>
#include <stack>


std::map<std::string, TYPE> OPERATORS = {
    {"cos", COS}, // косинус
    {"sin", SIN}, // синус
    {"tan", TAN}, // тангенс
    {"ctan", CTAN}, // котангенс
    {"exp", EXP}, // экспонента e^x
    {"^", POW},
    {"*", MULT},
    {"/", DIV},
    {"+", PLUS},
    {"-", MINUS},
    {"(", OPEN_BRACKET},
    {")", CLOSE_BRACKET}
};

std::map<TYPE, TYPE> OPER_TYPE = {
    {POW , BINAR},
    {MULT, BINAR},
    {DIV, BINAR},
    {PLUS, BINAR},
    {MINUS, BINAR},
    {COS, UNAR},
    {SIN, UNAR},
    {TAN, UNAR},
    {CTAN, UNAR},
    {EXP, UNAR}
};

std::map<TYPE, int> OPERATORS_PRIORITY = {
    {OPEN_BRACKET, -1},
    {CLOSE_BRACKET,-1},
    {COS, 3}, // косинус
    {SIN, 3}, // синус
    {TAN, 3}, // тангенс
    {CTAN, 3}, // котангенс
    {EXP, 3}, // экспонента e^x
    {POW, 2},
    {MULT, 1},
    {DIV, 1},
    {PLUS, 0},
    {MINUS, 0}
};

std::string NUMBERS = ".0123456789";

std::map<TYPE, float(*)(float, float)> functions = {
    {COS, [](float _, float b) -> float {return std::cos(b*M_PI/180.0);}},
    {SIN, [](float _, float b) -> float {return std::sin(b*M_PI/180.0);}},
    {TAN, [](float _, float b) -> float {return std::tan(b*M_PI/180.0);}},
    {CTAN, [](float _, float b) -> float {return 1/std::tan(b*M_PI/180.0);}},
    {POW, [](float a, float b) -> float {return std::pow(a, b);}},
    {EXP, [](float _, float b) -> float {return std::exp(b);}},
    {MULT, [](float a, float b) -> float {return a*b;}},
    {DIV, [](float a, float b) -> float {return a/b;}},
    {PLUS, [](float a, float b) -> float {return a+b;}},
    {MINUS, [](float a, float b) -> float {return a-b;}}
};

Token Calculator::str_to_token(std::string& string, std::size_t& position){
    int len = 1;
    while(position+len <= string.size()){
        if(OPERATORS.find(string.substr(position, len)) != OPERATORS.end()){
            Token token(OPERATORS[string.substr(position, len)]);
            position += len;
            return token;

        }else ++len;
    }
    return Token(ERROR);
}

std::vector<Token> Calculator::tokenisation(std::string expression){
    std::vector<Token> result;
    std::size_t pos = 0;

    while(pos != expression.size()){
        int len = 1;
        if(NUMBERS.find(expression.substr(pos, 1)) != NUMBERS.npos){
            std::size_t i{};
            float value = std::stof(expression.substr(pos), &i);
            result.push_back(Token(value));
            pos += i;
        }else{
            Token token = str_to_token(expression, pos);
            if(token.type!=ERROR){
                result.push_back(token);
            }else{
                std::size_t local_pos = pos;
                while( local_pos != expression.size()){
                    if(NUMBERS.find(expression[local_pos]) != NUMBERS.npos) break;
                    std::size_t i = local_pos;
                    token = str_to_token(expression, i);
                    if(token.type != ERROR) break;
                    ++local_pos;
                }
                result.push_back(Token(expression.substr(pos, local_pos-pos)));
                pos = local_pos;
            }
        }
    }
    return result;
}


float Calculator::calculate(std::string expression){
    // избавление от унарных минусов, синтаксическое дерево позволяет сделать это гораздо изящней
    if(expression[0] == '-') expression = "0" + expression;
    expression = std::regex_replace(expression, std::regex(R"(\(\-)"), "\(0-");

    float result{};
    std::vector<Token> input = tokenisation(expression);
    std::vector<Token> output;
    std::stack<Token> stack;
    for(int i{}; i < input.size(); ++i){  
        if(input[i].type == NUMB || input[i].type == VAR) output.push_back(input[i]);
        else if(input[i].type == OPEN_BRACKET) stack.push(input[i]);
        else if(input[i].type == CLOSE_BRACKET){
            while(stack.top().type != OPEN_BRACKET){
                output.push_back(stack.top());
                stack.pop();
            }
            stack.pop();
        }else if(stack.empty()) stack.push(input[i]);
        else{
            while(!stack.empty() && OPERATORS_PRIORITY[input[i].type] <= OPERATORS_PRIORITY[stack.top().type]){
                output.push_back(stack.top());
                stack.pop();
            }
            stack.push(input[i]);
        }
    }
    while(!stack.empty()){
        output.push_back(stack.top());
        stack.pop();
    }
    for(int i{}; i < output.size(); ++i){
        if(output[i].type == NUMB || output[i].type == VAR){
            stack.push(output[i]);
        }else{
            if(OPER_TYPE[output[i].type] == BINAR){
                float b = stack.top().value;
                stack.pop();
                float a = stack.top().value;
                stack.pop();
                stack.push(Token(functions[output[i].type](a,b)));
            }else{
                float b = stack.top().value;
                stack.pop();
                stack.push(Token(functions[output[i].type](0,b)));
            }
        }
    }



    return stack.top().value;

}
