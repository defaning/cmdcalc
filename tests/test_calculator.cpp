#include "calculator.h"
#include <iostream>
#include <cmath>

static bool approx(double a, double b, double eps=1e-9){ return std::isfinite(a) && std::fabs(a-b)<=eps; }

int main(){
    std::string err;
    double v;
    v = evaluate_expression("1+2", &err); if(!approx(v,3.0) || !err.empty()){ std::cerr<<"Test failed: 1+2 -> "<<v<<" err="<<err<<"\n"; return 1; }
    v = evaluate_expression("2*(3+4)", &err); if(!approx(v,14.0) || !err.empty()){ std::cerr<<"Test failed: 2*(3+4) -> "<<v<<" err="<<err<<"\n"; return 1; }
    v = evaluate_expression("-3+5", &err); if(!approx(v,2.0) || !err.empty()){ std::cerr<<"Test failed: -3+5 -> "<<v<<" err="<<err<<"\n"; return 1; }
    v = evaluate_expression("2^3^2", &err); if(!approx(v,512.0) || !err.empty()){ std::cerr<<"Test failed: 2^3^2 -> "<<v<<" err="<<err<<"\n"; return 1; }
    v = evaluate_expression("pi", &err); if(!approx(v,3.141592653589793) || !err.empty()){ std::cerr<<"Test failed: pi -> "<<v<<" err="<<err<<"\n"; return 1; }
    v = evaluate_expression("4/0", &err); if(err.empty()){ std::cerr<<"Test failed: 4/0 should error"<<"\n"; return 1; }
    std::cout<<"All tests passed"<<std::endl;
    return 0;
}
