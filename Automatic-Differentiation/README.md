# Automatic Differentiation

Very simple automatic differentiation tool, implemented using dual numbers and operator overloading.

This code is part of my article on **Medium** : **[Automatic Differentiation](https://medium.com/@omaraflak/automatic-differentiation-4d26d03b7508)**.

# Example

```c++
// Dual(value, derivative=0)
Dual x(5, 1); // derivative=1 means we are going to derive with respect to this variable.
Dual y(6);
Dual f = pow(x,2)*y; // the derivative is calculated when the function is computed.
std::cout << f.getDerivative() << std::endl; // get the derivative of y*x^2 with respect to x, evaluated at (x=5,y=6).
```
