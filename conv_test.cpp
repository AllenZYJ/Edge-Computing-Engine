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
#include <iomanip>

using namespace std;
clock_t start_t, end_t;
double duration;
int main()
{
    welcome();
    cout<<setprecision(13);
    Matrix3d mat_call = CreateMatrix3d(3, 64, 64);
    Matrix3d mat_call2 = CreateMatrix3d(3, 64, 64);
    edge_layer *conv2d_1 = new conv2d(CreateMatrix3d(3, 64, 64), 3, 12, 1, 2, 0, 0);
    edge_layer *conv2d_2 = new conv2d(CreateMatrix3d(12, 63, 63), 12, 24, 7, 7, 0, 0);
    edge_layer *conv2d_3 = new conv2d(CreateMatrix3d(24, 9, 9), 24, 128, 1, 3, 0, 0);
    start_t = clock();
    Matrix3d output_conv = conv2d_1->forward(mat_call2);
    Matrix3d output_conv2 = conv2d_2->forward(output_conv);
    Matrix3d output_conv3 = conv2d_3->forward(output_conv2);
    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("CPUduration：%f\n", total_t);
    getshape(output_conv3);
    Matrix b = ones(1,3);
    b.matrix[0][0] = 2;
    b.matrix[0][1] = 2;
    b.matrix[0][2] = 8;
    cout<<matrix_sum(matrix_power(subtract_ele(b,matrix_mean(b))))/2<<endl;
    cout<<matrix_var(b)<<endl;
    cout_mat(matrix_division(b,0.3));
    Matrix3d c = CreateMatrix3d(1,1,3);
    c.matrix3d[0] = b;
    edge_layer *bn_1 = new bn(0.1,0.2);
    Matrix3d output_bn1 = bn_1->forward(c);
    cout_mat3d(output_bn1);
    // cout<<matrix_var(b)<<endl;
    return 0;
}