#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "L&P/ExprToken.hpp"

std::string tokenTypeToString(TokenType type);
std::ostream &operator<<(std::ostream &os, TokenType type);

