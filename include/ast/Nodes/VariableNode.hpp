#pragma once
#include "ASTNode.hpp"
#include <iostream>
#include <string>

class VariableNode : public ASTNode
{
public:
    explicit VariableNode(std::string name ) : name_(name){}
    double eval(double u, double v) override
    {
        if (name_ == "u") return u;
        if (name_ == "v") return v;
        throw std::runtime_error("Unknown variable: " + name_);
    }
private:
    std::string name_;
};