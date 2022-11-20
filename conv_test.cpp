/*
███████╗██████╗  ██████╗ ███████╗                ███████╗███╗   ██╗ ██████╗ ██╗███╗   ██╗███████╗
██╔════╝██╔══██╗██╔════╝ ██╔════╝                ██╔════╝████╗  ██║██╔════╝ ██║████╗  ██║██╔════╝
█████╗  ██║  ██║██║  ███╗█████╗                  █████╗  ██╔██╗ ██║██║  ███╗██║██╔██╗ ██║█████╗
██╔══╝  ██║  ██║██║   ██║██╔══╝                  ██╔══╝  ██║╚██╗██║██║   ██║██║██║╚██╗██║██╔══╝
███████╗██████╔╝╚██████╔╝███████╗                ███████╗██║ ╚████║╚██████╔╝██║██║ ╚████║███████╗
╚══════╝╚═════╝  ╚═════╝ ╚══════╝                ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚═╝  ╚═══╝╚══════╝
Author：Edge
Web：likedge.top
Date：20200925
*/
#include <math.h>
#include <time.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "./autodiff/node.h"
#include "./file_pro/data_read.h"
#include "./grad_edge/matrix_grad.h"
#include "./logistic/logistic_def.h"
#include "./matrix/matrix_def.h"
#include "./matrix/matrix_pro.h"
#include "./welcome/score_wel.cpp"
using namespace std;
clock_t start, stop;
double duration;
int main() {
    welcome();
    conv_test(ones(3, 3), 3, 6, 1, 2, 0, 0);
    Matrix3d a = CreateMatrix3d(12, 3, 12);
    cout_mat3d(a);
    std::cout<<"------"<<endl;
    return 0;
}