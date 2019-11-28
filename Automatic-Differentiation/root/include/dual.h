#ifndef DUAL
#define DUAL

#include <iostream>
#include <cmath>

class Dual {
    private:
        double val;
        double der;

    public:
        Dual();
        Dual(double val);
        Dual(double val, double der);

        void setValue(double value);
        void setDerivative(double derivative);

        double getValue() const;
        double getDerivative() const;

        // operators
        friend Dual operator+(const Dual& u, const Dual& v);
        friend Dual operator-(const Dual& u, const Dual& v);
        friend Dual operator*(const Dual& u, const Dual& v);
        friend Dual operator/(const Dual& u, const Dual& v);

        Dual operator+=(const Dual& u);
        Dual operator-=(const Dual& u);
        Dual operator*=(const Dual& u);
        Dual operator/=(const Dual& u);

        friend std::ostream& operator<<(std::ostream& os, const Dual& a);

        // maths
        friend Dual sin(Dual d);
        friend Dual cos(Dual d);
        friend Dual exp(Dual d);
        friend Dual log(Dual d);
        friend Dual abs(Dual d);
        friend Dual pow(Dual d, double p);
};

#endif
