#pragma once

class Express
{
public:
    virtual double eval(double u, double v) const = 0;
    virtual ~Express() = default;
};