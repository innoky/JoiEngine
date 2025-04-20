#pragma once
#include "L&P/ExprToken.hpp"
#include "ast/expr.hpp"
#include <vector>
#include <memory>

class Parser
{
public:
    explicit Parser(const std::vector<ExprToken> &tokens);

    std::unique_ptr<Express> parse();

private:
    const std::vector<ExprToken> &tokens_;
    size_t pos_;

    const ExprToken &peek() const;
    const ExprToken &get();
    bool match(TokenType expected);

    
    std::unique_ptr<Express> parseExpression();
    std::unique_ptr<Express> parseTerm();
    std::unique_ptr<Express> parseFactor();
    std::unique_ptr<Express> parseBase();
};