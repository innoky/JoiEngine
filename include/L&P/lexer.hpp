#pragma once
#include <vector>
#include <string>
#include "L&P/ExprToken.hpp"

class Lexer {
public: 
    explicit Lexer(const std::string& content);

    void tokenize();
    std::vector<ExprToken> tokens;
    void printTokens();

private:
    std::string content_;
    size_t pos_;

    char peek() const;
    char get();
    void skipSpaces();

    ExprToken nextToken();

    ExprToken readNumber();
    ExprToken readIdentifierOrFunction();
};