#include "calculator.h"
#include "tokenizer.h"
#include <stack>
#include <vector>
#include <cmath>
#include <limits>

static int precedence(char op){
    switch(op){
        case 'n': return 5; // unary minus
        case '^': return 4;
        case '*': case '/': return 3;
        case '+': case '-': return 2;
    }
    return 0;
}
static bool is_right_assoc(char op){ return op=='^' || op=='n'; }

double evaluate_expression(const std::string &expr, std::string *err){
    std::string terr;
    auto tokens = tokenize(expr, &terr);
    if(tokens.empty() && !terr.empty()){ if(err) *err = terr; return std::numeric_limits<double>::quiet_NaN(); }

    // Convert to RPN using shunting-yard
    std::vector<Token> output;
    std::stack<Token> ops;
    for(size_t i=0;i<tokens.size();++i){
        Token t = tokens[i];
        if(t.type==TokenType::Number){ output.push_back(t); continue; }
        if(t.type==TokenType::Op){
            char op = t.op;
            // detect unary minus: if op is '-' and (start or previous is op or lparen)
            if(op=='-'){
                bool unary = false;
                if(i==0) unary = true;
                else{
                    Token prev = tokens[i-1];
                    if(prev.type==TokenType::Op || prev.type==TokenType::LParen) unary = true;
                }
                if(unary){ op = 'n'; }
            }
            // while top of ops has operator with greater precedence
            while(!ops.empty() && ops.top().type==TokenType::Op){
                char topop = ops.top().op;
                int p1 = precedence(op);
                int p2 = precedence(topop);
                if( (is_right_assoc(op) && p1 < p2) || (!is_right_assoc(op) && p1 <= p2) ){
                    output.push_back(ops.top()); ops.pop();
                } else break;
            }
            ops.push(Token{TokenType::Op, 0.0, op});
            continue;
        }
        if(t.type==TokenType::LParen){ ops.push(t); continue; }
        if(t.type==TokenType::RParen){
            bool found=false;
            while(!ops.empty()){
                if(ops.top().type==TokenType::LParen){ found=true; ops.pop(); break; }
                output.push_back(ops.top()); ops.pop();
            }
            if(!found){ if(err) *err = "Mismatched parentheses"; return std::numeric_limits<double>::quiet_NaN(); }
            continue;
        }
    }
    while(!ops.empty()){
        if(ops.top().type==TokenType::LParen || ops.top().type==TokenType::RParen){ if(err) *err = "Mismatched parentheses"; return std::numeric_limits<double>::quiet_NaN(); }
        output.push_back(ops.top()); ops.pop();
    }

    // Evaluate RPN
    std::stack<double> st;
    for(auto &t : output){
        if(t.type==TokenType::Number){ st.push(t.value); continue; }
        if(t.type==TokenType::Op){
            char op = t.op;
            if(op=='n'){
                if(st.empty()){ if(err) *err = "Insufficient operands for unary -"; return std::numeric_limits<double>::quiet_NaN(); }
                double a = st.top(); st.pop();
                st.push(-a);
                continue;
            }
            if(st.size()<2){ if(err) *err = "Insufficient operands"; return std::numeric_limits<double>::quiet_NaN(); }
            double b = st.top(); st.pop();
            double a = st.top(); st.pop();
            double res=0;
            switch(op){
                case '+': res = a+b; break;
                case '-': res = a-b; break;
                case '*': res = a*b; break;
                case '/': if(b==0.0){ if(err) *err = "Division by zero"; return std::numeric_limits<double>::quiet_NaN(); } res = a/b; break;
                case '^': res = std::pow(a,b); break;
                default: if(err) *err = std::string("Unknown operator: ")+op; return std::numeric_limits<double>::quiet_NaN();
            }
            st.push(res);
        }
    }
    if(st.size()!=1){ if(err) *err = "Malformed expression"; return std::numeric_limits<double>::quiet_NaN(); }
    return st.top();
}
