#include <iostream>

#include "../../root/include/vectmath.h"
#include "../../root/include/node.h"

Node function(std::vector<Node>& x){
    return pow(x[0], 2) + pow(x[1], 2); // x^2 + y^2
}

int main(int argc, char const *argv[]) {
    Graph* graph = Graph::getInstance();

    std::vector<Node> x = {50, 50};
    Node f;

    int epochs = 30;
    float learning_rate = 0.1;
    for(size_t i=0 ; i<epochs ; i++){
        f = function(x);
        x -= learning_rate*f.gradient(x);
        graph->new_recording();
    }

    std::cout << "f = " << f << std::endl;
    std::cout << "x = " << x << std::endl;
    return 0;
}
