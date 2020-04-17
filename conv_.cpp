#include<iostream>
#include<ctime>
#include<string>
#include <time.h>
#include <math.h>
#include <fstream>
#include "./autodiff/node.h"
#include"./matrix/matrix_def.h"
#include"./matrix/matrix_pro.h"
#include"./welcome/score_wel.cpp"
#include"./logistic/logistic_def.h"
#include"./file_pro/data_read.h"
#include"./grad_edge/matrix_grad.h"
using namespace std;
clock_t start, stop;
double duration;
int main()
{
    welcome();
    cout_mat(conv_test(CreateRandMat(3,3),1,2));
    return 0;

}