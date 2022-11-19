#include <iostream>
#include <iomanip>
#include <cassert>
#include <ctime>
#include <chrono>

#include "../../root/include/vectmath.h"
#include "../../root/include/node.h"

template <class T>
std::vector<std::vector<T> > get_random_matrix(const int& height, const int& width, T t){
    std::vector<std::vector<T> > mat(height, std::vector<T>(width));
    for(auto& v : mat){
        for(auto& e : v){
            e = rand()/(double)RAND_MAX;
        }
    }
    return mat;
}

int main(int argc, char const *argv[]) {
    srand(time(0));

    int size = 30;
    std::vector<std::vector<double> > a = get_random_matrix(size, size, double());
    std::vector<std::vector<double> > b = get_random_matrix(size, size, double());
    std::vector<std::vector<Node> > c = get_random_matrix(size, size, Node());
    std::vector<std::vector<Node> > d = get_random_matrix(size, size, Node());

    std::cout << std::fixed;
    std::cout << std::setprecision(10);

    std::cout << "Running with double...\t";
    std::cout.flush();
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<double> > ab = dot(a, b);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

    std::cout << "Running with Node...\t";
    std::cout.flush();
    start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<Node> > cd = dot(c, d);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

    std::cout << "Yet to be improved..." << std::endl;
    return 0;
}
