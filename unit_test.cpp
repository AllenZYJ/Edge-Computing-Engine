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
#include <iomanip>

using namespace std;

clock_t start_t, end_t;
double duration;
int main()
{
    welcome();
    cout_mat(CreateMatrix(10,9));
    Matrix3d a = conv_test_with_output(CreateMatrix3d(3,10,10),3,4,3,3,0,0);
    // cout_mat3d(a);
    return 0;
}