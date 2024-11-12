#include<string>

enum TYPE{
    COS,
    SIN,
    TAN,
    CTAN,
    EXP, // экспонента
    POW, // возведение в степень
    MULT,
    DIV,
    PLUS,
    MINUS,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    VAR,
    NUMB,
    ERROR,
    UNAR,
    BINAR
};

class Token{
public:
    TYPE type;
    float value;
    std::string var;
    Token(TYPE type);
    Token(float number);
    Token(std::string str);
};