#pragma once
#include <string>

enum class TokenType : short
{
    NUMBER,
    VARIABLE,
    FUNCTION,
    OPERATOR,
    LPAREN,
    RPAREN,
    PARAMETER,
    AXIS,
    END,
    UNKNOWN
};

struct ExprToken {
    std::string text;
    TokenType type;
};