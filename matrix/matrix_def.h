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
// CreateMatrix Create a Matrix for ones;
// ex
// 2*3
// 0 0 0
// 0 0 0
// change the value in matrix
#ifndef MATRIX_DEF
#define MATRIX_DEF
#pragma once
using namespace std;
typedef struct
{
    int row, col;
    float **matrix;
} Matrix;
typedef struct
{
    int row, col;
    string **str_matrix;
} str_Matrix;
typedef struct
{
    int dep, wid, high;
    Matrix *matrix3d;
} Matrix3d;
typedef struct
{
    int batch, dep, wid, high;
    Matrix3d *matrix4d;
} Matrix4d;
int change_va(Matrix Matrix, int index_x, int index_y, float value)
{
    Matrix.matrix[index_x][index_y] = value;
    return 0;
}
str_Matrix CreateStr_Ma(int ro, int co)
{
    str_Matrix str_arr;
    int row, col;
    string **designma;
    designma = (string **)malloc(ro * sizeof(string *));
    for (int i = 0; i < ro; i++)
    {
        designma[i] = (string *)malloc(co * sizeof(string));
    }
    for (int i = 0; i < ro; i++)
    {
        for (int j = 0; j < co; j++)
        {
            // cout<<"designma"<<designma[i][j]<<endl;
            designma[i][j] = "edge";
        }
    }
    str_arr.row = ro;
    str_arr.col = co;
    str_arr.str_matrix = designma;
    return str_arr;
}
Matrix CreateMatrix(int ro, int co)
{
    Matrix m;
    int row, col;
    float **inputMatrix;
    inputMatrix = (float **)malloc(ro * sizeof(float *));
    for (int i = 0; i < ro; i++)
    {
        inputMatrix[i] = (float *)malloc(co * sizeof(float));
    }
    for (int i = 0; i < ro; i++)
    {
        for (int j = 0; j < co; j++)
        {
            inputMatrix[i][j] = 0;
        }
    }
    m.col = co;
    m.row = ro;
    m.matrix = inputMatrix;
    return m;
}
Matrix CreateRandMat(int x_dim, int y_dim)
{
    int index_x, index_y;
    Matrix result = CreateMatrix(x_dim, y_dim);
    for (index_x = 0; index_x < x_dim; ++index_x)
    {
        for (index_y = 0; index_y < y_dim; ++index_y)
        {
            double temp_val = (rand() % 30000) * 0.0001 - 1;
            change_va(result, index_x, index_y, temp_val);
        }
    }
    return result;
}
Matrix ones(int ro, int co)
{
    Matrix m;
    int row, col;
    float **inputMatrix;
    inputMatrix = (float **)malloc(ro * sizeof(float *));
    for (int i = 0; i < ro; i++)
    {
        inputMatrix[i] = (float *)malloc(co * sizeof(float));
    }
    for (int i = 0; i < ro; i++)
    {
        for (int j = 0; j < co; j++)
        {
            inputMatrix[i][j] = 1;
        }
    }
    m.col = co;
    m.row = ro;
    m.matrix = inputMatrix;
    return m;
}
Matrix3d CreateMatrix3d(int depth, int width, int highth)
{
    Matrix3d m;
    int i, j, k;
    Matrix *Matrix_1;
    Matrix_1 = (Matrix *)malloc(depth * sizeof(Matrix));
    for (i = 0; i < depth; i++)
    {
        Matrix temp_matrix = CreateRandMat(width, highth);
        Matrix_1[i] = temp_matrix;
    }

    m.dep = depth;
    m.wid = width;
    m.high = highth;
    m.matrix3d = Matrix_1;
    return m;
}
Matrix4d CreateMatrix4d(int bat, int dep, int wid, int high)
{
    Matrix4d m;
    Matrix3d *inputMatrix;
    inputMatrix = (Matrix3d *)malloc(bat * sizeof(Matrix3d));
    for (int i = 0; i < bat; i++)
    {
        Matrix3d temp_mat3d = CreateMatrix3d(dep, wid, high);
        inputMatrix[i] = temp_mat3d;
    }
    m.batch = bat;
    m.dep = dep;
    m.wid = wid;
    m.high = high;
    m.matrix4d = inputMatrix;
    return m;
}
void getshape(Matrix3d mid1)
{
    cout << "EDGE Matrix Shape:(" << mid1.dep << "," << mid1.wid << "," << mid1.high << ")" << endl;
}

void cout_strmat(str_Matrix mid1)
{
    for (int index_x = 0; index_x < mid1.row; index_x++)
    {
        for (int index_y = 0; index_y < mid1.col; index_y++)
        {
            cout << mid1.str_matrix[index_x][index_y] << ",";
        }
        cout << endl;
    }
}

#endif
