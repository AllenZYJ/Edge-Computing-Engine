#include <iostream>
#include "../../root/include/node.h"

int main(int argc, char const *argv[]) {
    Node x=2, y=3;
    Node f = x*y + sin(x);

    std::cout << "f(x,y) = x*y + sin(x)" << std::endl;
    std::cout << "f(" << x << "," << y << ") = " << f << std::endl;
    std::cout << "∂f/∂x = " << f.gradient(x) << std::endl;
    std::cout << "∂f/∂y = " << f.gradient(y) << std::endl;
    return 0;
}
