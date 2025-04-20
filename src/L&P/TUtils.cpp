#include <iostream>
#include "TUtils/TUtils.hpp"

std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::VARIABLE:
        return "VARIABLE";
    case TokenType::FUNCTION:
        return "FUNCTION";
    case TokenType::OPERATOR:
        return "OPERATOR";
    case TokenType::LPAREN:
        return "LPAREN";
    case TokenType::RPAREN:
        return "RPAREN";
    case TokenType::PARAMETER:
        return "PARAMETER";
    case TokenType::AXIS:
        return "AXIS";
    case TokenType::END:
        return "END";
    case TokenType::UNKNOWN:
        return "UNKNOWN";
    default:
        return "???";
    }
}

std::ostream &operator<<(std::ostream &os, TokenType type)
{
    return os << tokenTypeToString(type);
}