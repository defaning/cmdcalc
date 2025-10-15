#pragma once

#include <string>
#include <vector>

enum class TokenType { Number, Op, LParen, RParen };

struct Token {
    TokenType type;
    double value;     // for numbers
    char op;          // for operators like + - * / ^ and 'n' for unary minus
};

std::vector<Token> tokenize(const std::string &s, std::string *err=nullptr);

// Set the last evaluated result so tokenizer can use it for 'ans'
void set_last_result(double value);
