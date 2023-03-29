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
    // cout_mat(CreateMatrix(10,9));
    // Matrix3d a = conv_test_with_output(CreateMatrix3d(3,10,8),3,4,2,3,0,0);

    Matrix3d mid1 = CreateMatrix3d(3, 4, 4);
    mid1.matrix3d[0] = CreateRandMat(4, 4);
    mid1.matrix3d[1] = CreateRandMat(4, 4);
    mid1.matrix3d[2] = CreateRandMat(4, 4);
    Matrix4d mid4 = CreateMatrix4d(2, 3, 4, 4);
    mid4.matrix4d[0] = mid1;
    mid4.matrix4d[1] = mid1;
    cout<<"input:";
    getshape4d(mid4);
    // 执行卷积计算
    Matrix4d output4d = batch_conv_test(mid4, 3, 13, 1, 2, 0, 1);
    getshape4d(output4d);

    // 打印结果
    cout << "Input: " << endl;
    cout_mat4d(mid4);
    getshape4d(mid4);
    cout << endl;
    cout << "Output: " << endl;
    cout_mat4d(output4d);
    cout << endl;

    // cout_mat3d(a);
    return 0;
}