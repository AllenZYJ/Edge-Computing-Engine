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
#include "./matrix/matrix_def.h"
#include "./matrix/matrix_pro.h"
#include "./logistic/logistic_def.h"
#include "./autodiff/node.h"
#include "./grad_edge/matrix_grad.h"

#include "./root/include/edgelayer.h"
#include "./welcome/score_wel.cpp"
#include "./file_pro/data_read.h"
using namespace std;
clock_t start_t, end_t;
double duration;
int main()
{
    welcome();
    Matrix3d a = CreateMatrix3d(3, 3, 5);
    cout_mat3d(a);
    start_t = clock();
    Matrix3d result = conv_test(CreateMatrix3d(3, 64, 64), 3, 6, 2, 3, 0, 0);
    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("CPU duration：%f\n", total_t);
    getshape(result);
    Matrix4d mat4d = CreateMatrix4d(2, 3, 3, 3);
    cout_mat3d(mat4d.matrix4d[0]);
    mat4d.matrix4d[0] = a;
    cout_mat3d(mat4d.matrix4d[0]);
    return 0;
}