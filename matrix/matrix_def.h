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
typedef struct {
    int row, col;
    float **matrix;
} Matrix;
typedef struct {
    int row, col;
    string **str_matrix;
} str_Matrix;
typedef struct {
    int dep, wid, high;
    float ***matrix3d;
} Matrix3d;
str_Matrix CreateStr_Ma(int ro, int co) {
    str_Matrix str_arr;
    int row, col;
    string **designma;
    designma = (string **)malloc(ro * sizeof(string *));
    for (int i = 0; i < ro; i++) {
        designma[i] = (string *)malloc(co * sizeof(string));
    }
    for (int i = 0; i < ro; i++) {
        for (int j = 0; j < co; j++) {
            // cout<<"designma"<<designma[i][j]<<endl;
            designma[i][j] = "edge";
        }
    }
    str_arr.row = ro;
    str_arr.col = co;
    str_arr.str_matrix = designma;
    return str_arr;
}
Matrix CreateMatrix(int ro, int co) {
    Matrix m;
    int row, col;
    float **inputMatrix;
    inputMatrix = (float **)malloc(ro * sizeof(float *));
    for (int i = 0; i < ro; i++) {
        inputMatrix[i] = (float *)malloc(co * sizeof(float));
    }
    for (int i = 0; i < ro; i++) {
        for (int j = 0; j < co; j++) {
            inputMatrix[i][j] = 0;
        }
    }
    m.col = co;
    m.row = ro;
    m.matrix = inputMatrix;
    return m;
}
void dele_matrix(Matrix3d mat3d){
    int i,j,k;
       for(j = 0 ; j <mat3d.wid  ; j++)
       {
              for(k = 0 ; k < mat3d.high ;k++)
              {
                    // std::cout<<mat3d.matrix3d[i][j][k]<<endl;
                    free(mat3d.matrix3d[j][k]);
              }
       }
    for(i = 0 ; i < mat3d.dep ; i++)
    {
        free(mat3d.matrix3d[i]);

    }
    free(mat3d.matrix3d);
}

Matrix3d CreateMatrix3d(int depth, int width, int highth) {
    Matrix3d m;
    int i, j, k;
    float ***Matrix_1;
    Matrix_1 = (float ***)malloc(depth * sizeof(float **));
    for (i = 0; i < depth; i++) {
        *(Matrix_1 + i) = (float **)malloc(width * sizeof(float *));
    }
    for (i = 0; i < depth; i++) {
        for (j = 0; j < width; j++) {
            *(*(Matrix_1 + i) + j) = (float *)malloc(highth * sizeof(float));
        }
    }
    for (i = 0; i < depth; i++) {
        for (j = 0; j < width; j++) {
            for (k = 0; k < highth; k++) {
                Matrix_1[i][j][k] = 0.0;
            }
        }
    }

    m.dep = depth;
    m.wid = width;
    m.high = highth;
    m.matrix3d = Matrix_1;
    return m;
}
void getshape(Matrix3d mid1){
    cout<<"EDGE Matrix Shape:("<<mid1.dep<<","<<mid1.wid<<","<<mid1.high<<")"<<endl;
}
Matrix ones(int ro, int co) {
    Matrix m;
    int row, col;
    float **inputMatrix;
    inputMatrix = (float **)malloc(ro * sizeof(float *));
    for (int i = 0; i < ro; i++) {
        inputMatrix[i] = (float *)malloc(co * sizeof(float));
    }
    for (int i = 0; i < ro; i++) {
        for (int j = 0; j < co; j++) {
            inputMatrix[i][j] = 1;
        }
    }
    m.col = co;
    m.row = ro;
    m.matrix = inputMatrix;
    return m;
}

void cout_strmat(str_Matrix mid1) {
    for (int index_x = 0; index_x < mid1.row; index_x++) {
        for (int index_y = 0; index_y < mid1.col; index_y++) {
            cout << mid1.str_matrix[index_x][index_y] << ",";
        }
        cout << endl;
    }
}
int change_va(Matrix Matrix, int index_x, int index_y, float value) {
    Matrix.matrix[index_x][index_y] = value;
    return 0;
}

#endif
