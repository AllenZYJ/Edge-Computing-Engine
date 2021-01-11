#include <iostream>

#include "../../root/include/vectmath.h"
#include "../../root/include/node.h"

Node function(std::vector<Node> x){
    return pow(x[0]-x[1], 2) + x[0]*x[1]*x[2]; // (x-y)^2 + x*y*z
}

int main(int argc, char const *argv[]) {
    std::vector<Node> x = {5,6,7};
    Node f = function(x);
    std::cout << "grad(f) = " << f.gradient(x) << std::endl;
    return 0;
}
