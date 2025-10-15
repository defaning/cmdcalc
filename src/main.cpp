#include "calculator.h"
#include <iostream>
#include <string>

int main(int argc, char **argv){
    if(argc>1){
        std::string expr;
        for(int i=1;i<argc;++i){ if(i>1) expr += " "; expr += argv[i]; }
        std::string err;
        double v = evaluate_expression(expr, &err);
        if(!err.empty()){ std::cerr<<"Error: "<<err<<"\n"; return 2; }
        std::cout<<v<<"\n";
        return 0;
    }
    // REPL
    std::cout<<"cmdcalc - type expression, or 'quit'/'exit' to leave"<<std::endl;
    std::string line;
    while(true){
        std::cout<<"> ";
        if(!std::getline(std::cin, line)) break;
        if(line=="quit" || line=="exit") break;
        if(line.empty()) continue;
        std::string err;
        double v = evaluate_expression(line, &err);
        if(!err.empty()) std::cout<<"Error: "<<err<<"\n";
        else std::cout<<v<<"\n";
    }
    return 0;
}
