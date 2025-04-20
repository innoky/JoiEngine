#pragma once

class ASTNode
{
public:
    virtual double eval(double u, double v) = 0;
    virtual ~ASTNode() {};
};

