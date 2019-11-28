#include <iostream>
#include "../include/dual.h"
#include "../include/gradient.h"

typedef std::vector<Dual> Vector;

std::ostream& operator<<(std::ostream& os, const Vector& v){
    os << "[";
    for(size_t i=0 ; i<v.size() ; i++){
        os << v[i] << (i<v.size()-1?", ":"");
    }
    os << "]";
    return os;
}

Dual function(Vector& x){
    return x[0]*x[0]+x[1]*x[1]; // f(x1,x2) = x1^2 + x2^2
}

int main() {
    
    Vector x = {50, 70}; // random default values

    // hyperparameters
    int epochs = 30;
    float learningRate = 0.3;

    // gradient descent
    for(size_t i=0 ; i<epochs ; i++){
        Vector gradient = dual::gradient(function, x);
        for(size_t j=0 ; j<x.size() ; j++){
            x[j] -= learningRate*gradient[j];
        }
    }

    // check new values
    std::cout << "x = " << x << std::endl;
    std::cout << "f(x) = " << function(x) << std::endl;
    
    return 0;
}
