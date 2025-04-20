#pragma once
#include "ASTNode.hpp"

class NumberNode : public ASTNode
{
public:
    explicit NumberNode(double value) : value_(value) {}
    double eval(double u, double v) override {return value_;};
private:
    double value_;
};