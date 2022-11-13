#ifndef VECTMATH
#define VECTMATH

#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <functional>

// dot product
template <typename T>
std::vector<std::vector<T> > dot(const std::vector<std::vector<T> >& a, const std::vector<std::vector<T> >& b){
    assert(a[0].size()==b.size());

    T w=0;
    std::vector<std::vector<T> > result(a.size(), std::vector<T>(b[0].size()));
    for (int i=0 ; i<a.size() ; i++){
        for (int j=0 ; j<b[0].size() ; j++){
            for (int h=0 ; h<b.size() ; h++){
                w += a[i][h]*b[h][j];
            }
            result[i][j] = w;
            w=0;
        }
    }

    return result;
}

// operators
template <typename U, typename V>
std::vector<U>& operator-=(std::vector<U>& u, const std::vector<V>& v){
    assert(u.size()==v.size());
    for(size_t i=0 ; i<u.size() ; i++){
        u[i] -= v[i];
    }
    return u;
}

template <typename U>
std::vector<U> operator+(const std::vector<U>& u, const std::vector<U>& v){
    assert(u.size()==v.size());
    std::vector<U> w(u.size());
    for(size_t i=0 ; i<w.size() ; i++){
        w[i] = u[i]+v[i];
    }
    return w;
}

template <typename U, typename S>
std::vector<U> operator*(const S& s, const std::vector<U>& u){
    std::vector<U> result(u.size());
    for(size_t i=0 ; i<u.size() ; i++){
        result[i] = s*u[i];
    }
    return result;
}

template <typename U>
std::vector<U>& operator>>(U (*fun)(U&), std::vector<U>& u){
    std::transform(u.begin(), u.end(), u.begin(), fun);
    return u;
}

template <typename U, typename S>
std::vector<U>& operator>>(S (*fun)(S&), std::vector<U>& u){
    for(auto& v : u){
        fun >> v;
    }
    return u;
}

template <typename U>
std::vector<U>& operator>>(U (*fun)(), std::vector<U>& u){
    for(auto& e : u){
        e = fun();
    }
    return u;
}

template <typename U, typename S>
std::vector<U>& operator>>(S (*fun)(), std::vector<U>& u){
    for(auto& v : u){
        fun >> v;
    }
    return u;
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const std::vector<U>& u){
    os << "[";
    for(size_t i=0 ; i<u.size() ; i++){
        os << u[i] << (i<u.size()-1?", ":"");
    }
    os << "]";
    return os;
}

#endif
