#pragma once
#include "ASTNode.hpp"
#include <iostream>
#include <string>

class OperatorNode : public ASTNode
{
public:
    explicit OperatorNode(std::string name) : name_(name) {}
    double eval(double u, double v) override
    {
    }

private:
    std::string name_;
};