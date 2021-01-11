# Autodiff

Reverse automatic differentiation tool in C++.

# Examples

Examples you may find helpful.

* [Neural Network](https://github.com/OmarAflak/autodiff/blob/master/examples/src/ann.cpp)
* [Gradient Descent](https://github.com/OmarAflak/autodiff/blob/master/examples/src/gradient_descent.cpp)
* [Derivatives](https://github.com/OmarAflak/autodiff/blob/master/examples/src/simple.cpp)
* [Gradient](https://github.com/OmarAflak/autodiff/blob/master/examples/src/gradient.cpp)

# Install & Use

Normal make sequence

```
make
make install
```

Then in a new `main.cpp` file :

```c++
#include <iostream>
#include <autodiff/node.h>

int main(int argc, char const *argv[]) {
    Node x=2, y=3;
    Node f = x*y + sin(x);

    std::cout << "f(x,y) = x*y + sin(x)" << std::endl;
    std::cout << "f(" << x << "," << y << ") = " << f << std::endl;
    std::cout << "∂f/∂x = " << f.gradient(x) << std::endl;
    std::cout << "∂f/∂y = " << f.gradient(y) << std::endl;
    return 0;
}
```

Compile it :

```
g++ main.cpp -o main -lautodiff
```

# Clear the computational graph

You should clear the computational graph whenever a new calculation happens.

```c++
Graph::getInstance()->new_recording();
```

# Operations Supported

### Trigonometric

* sin(const Node& x)
* cos(const Node& x)
* tan(const Node& x)
* asin(const Node& x)
* acos(const Node& x)
* atan(const Node& x)

### Hyperbolic

* sinh(const Node& x)
* cosh(const Node& x)
* tanh(const Node& x)

### Exponential and logarithmic

* log(const Node& x, const Node& base)
* log10(const Node& x)
* ln(const Node& x)
* exp(const Node& x)

### Power

* pow(const Node& x, const Node& p)
* sqrt(const Node& x)

### Others

* abs(const Node& x)
* min(const Node& l, const Node& r)
* max(const Node& l, const Node& r)

## Define your own operations

If your calculations involve functions I didn't mention above, then you can write your own operations and provide their derivative.

Thus, neither of the examples below needs to be implemented this way, they use operations already defined (*,+,exp). There are just for the sake of example.

### Monadic operations

<img src="https://latex.codecogs.com/gif.latex?\inline&space;f&space;:&space;R&space;\mapsto&space;R" title="f : R \mapsto R" />

```c++
Node multBy5(const Node& node){
    return Node::monadic_operation(node, [](const double& x){
        return MonadicOperationResult(5*x, 5); // (value=5*x, d(multBy5)/dx=5)
    });
}
```

### Dyadic operations

<img src="https://latex.codecogs.com/gif.latex?\inline&space;f&space;:&space;R^2&space;\mapsto&space;R" title="f : R^2 \mapsto R" />

```c++
Node multiply(const Node& left, const Node& right){
    return Node::dyadic_operation(left, right, [](const double& l, const double& r){
        return DyadicOperationResult(l*r, r, l); // (value=l*r, ∂(multiply)/∂l=r, ∂(multiply)/∂r=l)
    });
}
```

### Polyadic operations

<img src="https://latex.codecogs.com/gif.latex?\inline&space;f&space;:&space;R^n&space;\mapsto&space;R" title="f : R^n \mapsto R" />

```c++
Node someRandomFunction(const std::vector<Node>& nodes){
    return Node::polyadic_operation(nodes, [](const std::vector<double>& x){
        double result=0;
        for(const double& d : x){
            result += exp(d);
        }

        std::vector<double> gradients(nodes.size());
        for(size_t i=0 ; i<gradients.size() ; i++){
            gradients[i] = exp(x[i]);
        }

        return PolyadicOperationResult(result, gradients);
    });
}
```
