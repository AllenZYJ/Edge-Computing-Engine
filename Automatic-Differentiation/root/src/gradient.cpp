#include "../include/gradient.h"

void dual::resetDerivatives(dual::Vector& input){
    for(auto& e : input){
        e.setDerivative(0);
    }
}

dual::Vector dual::gradient(Dual (*f)(dual::Vector&), dual::Vector& input){
    resetDerivatives(input);
    dual::Vector grad(input.size());
    for(size_t i=0 ; i<grad.size() ; i++){
        input[i].setDerivative(1);
        grad[i] = f(input).getDerivative();
        input[i].setDerivative(0);
    }
    return grad;
}

dual::Matrix dual::jacobian(dual::Vector (*f)(dual::Vector&), dual::Vector& input){
    resetDerivatives(input);
    dual::Matrix jac(f(input).size(), dual::Vector(input.size()));
    for(size_t i=0 ; i<jac.size() ; i++){
        for(size_t j=0 ; j<jac[i].size() ; j++){
            input[j].setDerivative(1);
            jac[i][j] = f(input)[i].getDerivative();
            input[j].setDerivative(0);
        }
    }
    return jac;
}
