#include <math.h>
#include <time.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include "../matrix/matrix_def.h"
#include "../matrix/matrix_pro.h"
#include "../logistic/logistic_def.h"
#include "../autodiff/node.h"
#include "../grad_edge/matrix_grad.h"
#include "../root/include/edgelayer.h"
#include "../welcome/score_wel.cpp"
using namespace std;

clock_t start_t, end_t;
double duration;
// #include"opencv4/opencv2/imgcodecs.hpp"
// #include <opencv2/imgcodecs.hpp>
// using namespace cv;
int test_fetch_gap_one()
{
    Matrix3d input = CreateMatrix3d(3, 8, 8);
    cout_mat3d(input);
    Matrix4d output = fetch_skipOne(input);
    cout_mat4d(output);
}

int main()
{
    welcome();
    test_fetch_gap_one();
}