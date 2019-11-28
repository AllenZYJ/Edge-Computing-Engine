#include<iostream>
#include<ctime>
#include<string>
#include <time.h>
#include <math.h>
#include <fstream>
#include"./autodiff/node.h"
#include"./matrix/matrix_def.h"
#include"./matrix/matrix_pro.h"
#include"./welcome/score_wel.cpp"
#include"./logistic/logistic_def.h"
#include"./file_pro/data_read.h"
using namespace std;
int main()
{
	Node u=2, p=3,tt = 10;
    Node f = u*p*p+exp(tt)+cos(u) + sin(p);
    std::cout << "∂f/∂c = " << f.gradient(tt) << std::endl;
    std::cout << "∂f/∂y = " << f.gradient(p) << std::endl;
}
