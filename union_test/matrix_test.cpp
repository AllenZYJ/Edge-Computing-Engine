#include <iostream>
#include <ctime>
#include <string>
#include <time.h>
#include <math.h>
#include <fstream>
#include "../matrix/matrix_def.h"
#include "../welcome/score_wel.cpp"
#include "../matrix/matrix_pro.h"
struct testMatrix
{
    int row = 100, col = 100;
    float **matrix;
} Matrix_1;
int main()
{
    Matrix a = ones(4, 4);
    Matrix b = CreateRandMat(4, 4);
    str_Matrix c = CreateStr_Ma(4, 4);
    cout_mat(a);
    cout << "=----------=" << endl;
    cout_mat(b);
    cout << "=----------=" << endl;
    cout_strmat(c);
    cout << "=----------=" << endl;
    cout << Matrix_1.row << endl;
    return 0;
}
