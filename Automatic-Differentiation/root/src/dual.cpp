#include "../include/dual.h"

Dual::Dual(){
    this->val = 0;
    this->der = 0;
}

Dual::Dual(double val){
    this->val = val;
    this->der = 0;
}

Dual::Dual(double val, double der){
    this->val = val;
    this->der = der;
}

void Dual::setValue(double value){
    this->val = value;
}

void Dual::setDerivative(double derivative){
    this->der = derivative;
}

double Dual::getValue() const{
    return val;
}

double Dual::getDerivative() const{
    return der;
}

// operators
Dual operator+(const Dual& u, const Dual& v){
    return Dual(u.val+v.val, u.der+v.der);
}

Dual operator-(const Dual& u, const Dual& v){
    return Dual(u.val-v.val, u.der-v.der);
}

Dual operator*(const Dual& u, const Dual& v){
    return Dual(u.val*v.val, u.der*v.val+u.val*v.der);
}

Dual operator/(const Dual& u, const Dual& v){
    return Dual(u.val/v.val, (u.der*v.val-u.val*v.der)/(v.val*v.val));
}

Dual Dual::operator+=(const Dual& u){
    *this = *this+u;
    return *this;
}

Dual Dual::operator-=(const Dual& u){
    *this = *this-u;
    return *this;
}

Dual Dual::operator*=(const Dual& u){
    *this = *this*u;
    return *this;
}

Dual Dual::operator/=(const Dual& u){
    *this = *this/u;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Dual& a){
    os << a.val;
    return os;
}

// maths
Dual sin(Dual d){
    return Dual(::sin(d.val), d.der*::cos(d.val));
}

Dual cos(Dual d){
    return Dual(::cos(d.val), -d.der*::sin(d.val));
}

Dual exp(Dual d){
    return Dual(::exp(d.val), d.der*::exp(d.val));
}

Dual log(Dual d){
    return Dual(::log(d.val), d.der/d.val);
}

Dual abs(Dual d){
    int sign = d.val==0 ? 0 : d.val/::abs(d.val);
    return Dual(::abs(d.val), d.der*sign);
}

Dual pow(Dual d, double p){
    return Dual(::pow(d.val, p), p*d.der*::pow(d.val, p-1));
}
