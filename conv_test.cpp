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
// #include <iomanip>

using namespace std;

clock_t start_t, end_t;
double duration;
// #include"opencv4/opencv2/imgcodecs.hpp"
// #include <opencv2/imgcodecs.hpp>
// using namespace cv;

int main()
{
    welcome();
    cout << setprecision(13);
    start_t = clock();
    Matrix4d mat_call = CreateMatrix4d(2, 3, 640, 640);
    print_shape(mat_call);

    edge_layer *conv2d_1 = new conv2d(mat_call, 3, 12, 1, 2, 0, 0);
    Matrix4d output_conv = conv2d_1->forward(mat_call);
    print_shape(output_conv);    
    int parameter_1 = conv2d_1->parameter_counter();

    edge_layer *conv2d_2 = new conv2d(output_conv, 12, 24, 7, 7, 0, 0);
    Matrix4d output_conv2 = conv2d_2->forward(output_conv);
    print_shape(output_conv2);
    std::cout << "EDGE Matrix Shape: (" << output_conv2.batch << "," << output_conv2.dep << ","
              << output_conv2.wid << "," << output_conv2.high << ")" << std::endl;
    parameter_1 += conv2d_2->parameter_counter();

    edge_layer *conv2d_3 = new conv2d(output_conv2, 24, 1, 3, 3, 0, 0);
    Matrix4d output_conv3 = conv2d_3->forward(output_conv2);
    print_shape(output_conv3);
    parameter_1 += conv2d_3->parameter_counter();
    end_t = clock();
    cout<<"total parameters :"<<parameter_1<<endl;
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("CPUduration：%f\n", total_t);
    

}