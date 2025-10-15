#include "tokenizer.h"
#include <cctype>
#include <cstdlib>

std::vector<Token> tokenize(const std::string &s, std::string *err){
    std::vector<Token> out;
    size_t i=0;
    while(i<s.size()){
        char c = s[i];
        if(std::isspace((unsigned char)c)){ i++; continue; }
        if((c>='0' && c<='9') || c=='.'){
            size_t j=i;
            while(j<s.size() && (std::isdigit((unsigned char)s[j]) || s[j]=='.')) j++;
            std::string tok = s.substr(i, j-i);
            char *endptr=nullptr;
            double v = std::strtod(tok.c_str(), &endptr);
            if(endptr==tok.c_str()){
                if(err) *err = "Invalid number: "+tok;
                return {};
            }
            out.push_back(Token{TokenType::Number, v, 0});
            i = j;
            continue;
        }
        if(c=='+'||c=='-'||c=='*'||c=='/'||c=='^'){
            out.push_back(Token{TokenType::Op, 0.0, c});
            i++; continue;
        }
        if(c=='('){ out.push_back(Token{TokenType::LParen, 0.0, 0}); i++; continue; }
        if(c==')'){ out.push_back(Token{TokenType::RParen, 0.0, 0}); i++; continue; }
        // constants: pi and e
        if(std::isalpha((unsigned char)c)){
            size_t j=i;
            while(j<s.size() && std::isalpha((unsigned char)s[j])) j++;
            std::string name = s.substr(i, j-i);
            if(name=="pi") out.push_back(Token{TokenType::Number, 3.141592653589793, 0});
            else if(name=="e") out.push_back(Token{TokenType::Number, 2.718281828459045, 0});
            else { if(err) *err = "Unknown identifier: "+name; return {}; }
            i = j; continue;
        }
        if(err) *err = std::string("Unexpected character: ")+c;
        return {};
    }
    return out;
}
