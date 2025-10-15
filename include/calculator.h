#pragma once

#include <string>

// Evaluate expression. On error, returns NaN and sets err (if provided).
double evaluate_expression(const std::string &expr, std::string *err=nullptr);
