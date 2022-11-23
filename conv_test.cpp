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
    // welcome();
    Matrix3d mat_call = CreateMatrix3d(3, 64, 64);
    edge_layer *conv2d_1 = new conv2d(CreateMatrix3d(3, 64, 64), 3, 6, 1, 2, 0, 0);
    if (conv2d_1 != NULL)
    {
        start_t = clock();
        Matrix3d output_conv = conv2d_1->layer_call(mat_call);
        end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        printf("CPU duration：%f\n", total_t);
        // cout_mat3d(output_conv);
        getshape(output_conv);
        conv2d_1 = NULL;
    }
    return 0;
}