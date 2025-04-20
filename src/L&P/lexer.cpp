#include <iostream>
#include "L&P/lexer.hpp"
#include "TUtils/TUtils.hpp"


Lexer::Lexer(const std::string& content)
{
    content_ = content;
    pos_ = 0;
}

char Lexer::peek() const 
{
    if (pos_ >= content_.size())
    {
        return '\0';
    }
    else {
        return content_[pos_];
    }
}

char Lexer::get() 
{
    if (pos_ >= content_.size())
    {
        return '\0';
    }
    else {
        return content_[pos_++];
    }
}

void Lexer::skipSpaces() 
{
    while (pos_ < content_.size() && content_[pos_] == ' ')
    {pos_++;}
}

ExprToken Lexer::nextToken() 
{
    skipSpaces();

    char current = peek();

    if (current == '\0') {
        return ExprToken{"", TokenType::END};
    }
    else if (isdigit(current) || current == '.')
    {
        return readNumber();
    }
    else if (current == '+'
    ||  current == '-'
    ||  current == '*'
    ||  current == '/'
    ||  current == '^'
    ||  current == '=' ) {
        get();
        return ExprToken{std::string(1, current), TokenType::OPERATOR};
    }
    else if (current == '(')
    {
        get();
        return ExprToken{"(", TokenType::LPAREN};
    }
    else if (current == ')')
    {
        get();
        return ExprToken{")", TokenType::RPAREN};
    }
    else if (isalpha(current))
    {
        return readIdentifierOrFunction();
    }
    else
    {
        get();
        return ExprToken{std::string(1, current), TokenType::UNKNOWN};
    }
    
    return ExprToken{std::string(1, current), TokenType::END};
}

ExprToken Lexer::readNumber() 
{
    std::string numberStr = "";

    bool hasDot = false;

    while (pos_ < content_.size())
    {
        char c = peek();
        if (isdigit(c)) 
        {
            numberStr += get();
        }
        else if (c == '.')
        {
            numberStr += get();
            hasDot = true;
        }
        else
        {
            break;
        }
    }
    return ExprToken{numberStr, TokenType::NUMBER};
}

ExprToken Lexer::readIdentifierOrFunction()
{
    std::string exp;
    while (pos_ < content_.size() && isalpha(content_[pos_]))
    {
        exp+=get();
    }

    if (exp == "u" || exp == "v")
    {
        return ExprToken{exp, TokenType::VARIABLE};
    }
    else if (exp == "x" || exp == "y" || exp == "z")
    {
        return ExprToken{exp, TokenType::AXIS};
    }
    else if (exp.size() == 1)
    {
        return ExprToken{exp, TokenType::PARAMETER};
    }
    else if (exp == "sin" || exp == "cos" || exp == "tan" || exp == "exp" || exp == "log")
    {
            return ExprToken{exp, TokenType::FUNCTION};
    }
    else
    {
        return ExprToken{exp, TokenType::UNKNOWN}; 
    }
}

void Lexer::tokenize()
{
    tokens.clear();
    pos_ = 0;
    while (pos_ < content_.size())
    {
        ExprToken token = nextToken();
        tokens.push_back(token);
    }
}

void Lexer::printTokens()
{   
    for (int id = 0; id < tokens.size(); id++)
    {
        std::cout << tokens[id].text << " | ";
        std::cout << tokens[id].type << std::endl;
    }
}

