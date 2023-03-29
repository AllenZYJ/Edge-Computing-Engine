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
    Matrix3d a = conv_test_with_output(CreateMatrix3d(3,10,8),3,4,2,3,0,0);
    srand((unsigned)time(NULL));
    Matrix4d X = CreateMatrix4d(2, 3, 3, 3);
    Matrix4d X_padding = CreatePadding4d(X,2);
    Matrix4d W = CreateMatrix4d(32, 3, 5, 5);
    Matrix4d dZ = CreateMatrix4d(10, 32, 24, 24);
    for(int i=0;i<X.batch;i++)
        {cout_mat3d(X_padding.matrix4d[i]);}
    // cout_mat3d(a);
    return 0;
}