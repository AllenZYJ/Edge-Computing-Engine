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
void free_mat(Matrix mat)
{
    for (int i = 0; i < mat.row; i++)
    {
        free(mat.matrix[i]);
    }
    free(mat.matrix);
}
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
Matrix zeros(int ro, int co)
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
Matrix3d CreateMatrix3d(int depth, int height, int width)
{
    Matrix3d m;
    int i, j, k;
    Matrix *Matrix_1;
    Matrix_1 = (Matrix *)malloc(depth * sizeof(Matrix));
    for (i = 0; i < depth; i++)
    {
        Matrix temp_matrix = CreateRandMat(width, height);
        Matrix_1[i] = temp_matrix;
    }

    m.dep = depth;
    m.wid = height;
    m.high = width;
    // exchange the value
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
// void getshape4d(Matrix4d m) {
//     cout << "EDGE Matrix Shape: (" << m.batch << "," << m.dep << "," << m.wid << "," << m.high << ")" << endl;
//     // return m.batch,m.dep,m.wid,m.high
// }
struct Matrix4dDimensions
{
    int batch;
    int dep;
    int wid;
    int high;
};

Matrix4dDimensions getshape4d(Matrix4d m)
{
    std::cout << "EDGE Matrix Shape: (" << m.batch << "," << m.dep << ","
              << m.wid << "," << m.high << ")" << std::endl;
    return {m.batch, m.dep, m.wid, m.high};
}
void print_shape(const Matrix4d &matrix)
{
    std::cout << "EDGE Matrix Shape: (" << matrix.batch << "," << matrix.dep << ","
              << matrix.wid << "," << matrix.high << ")" << std::endl;
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
Matrix4d fetch_skipOne(Matrix3d m)
{
    /**
     * This function takes a 3D matrix input m, skips every other row and column in each matrix slice,
     * and outputs a 4D matrix containing the skipped values in separate slices.
     *
     * @param m - The input 3D matrix
     * @return A 4D matrix where each slice contains the skipped values from the corresponding slice in m
     */

    /*
    The logic:
    1. Get the dimensions of the input 3D matrix m
    2. Create 4 output 3D matrices to hold the skipped values
    3. Loop through each slice (depth) in m
       3a. Create a 2D matrix to hold the skipped values for this slice
       3b. Loop through every other row and column in this slice, copying the skipped values into the 2D matrix
       3c. Add the 2D matrix as a slice in the corresponding output 3D matrix
    4. Create the final 4D output matrix
    5. Add the 4 output 3D matrices as slices in the 4D matrix
    6. Return the 4D matrix
    */

    /*
    The 4D output matrix will have:
    - 4 slices, each containing:
      - The same depth as the input m
      - Half the original width and height (since we skip every other row/column)
    - Slice 0 contains values skipped in the first rows/columns
    - Slice 1 contains values skipped in the second rows/columns
    - etc.
    */
    int new_depth = m.dep;
    int new_height = m.wid;
    int new_width = m.high;
    Matrix3d output1 = CreateMatrix3d(new_depth, new_width, new_height);
    Matrix3d output2 = CreateMatrix3d(new_depth, new_width, new_height);
    Matrix3d output3 = CreateMatrix3d(new_depth, new_width, new_height);
    Matrix3d output4 = CreateMatrix3d(new_depth, new_width, new_height);
    cout << new_height << endl;
    cout << new_width << endl;
    for (int i = 0; i < new_depth; i++)
    {
        Matrix new_matrix = CreateMatrix(new_width / 2, new_height / 2);
        int idx_x = 0;
        for (int j = 0; j < new_height; j += 2)
        {
            int idx_y = 0;
            for (int k = 0; k < new_width; k += 2)
            {
                new_matrix.matrix[idx_x][idx_y] = m.matrix3d[i].matrix[j][k];
                idx_y += 1;
            }
            idx_x += 1;
        }
        output1.matrix3d[i] = new_matrix;
    }
    for (int i = 0; i < new_depth; i++)
    {
        Matrix new_matrix = CreateMatrix(new_width / 2, new_height / 2);
        int idx_x = 0;
        for (int j = 0; j < new_height; j += 2)
        {
            int idx_y = 0;
            for (int k = 1; k < new_width; k += 2)
            {
                new_matrix.matrix[idx_x][idx_y] = m.matrix3d[i].matrix[j][k];
                idx_y += 1;
            }
            idx_x += 1;
        }
        output2.matrix3d[i] = new_matrix;
    }
    for (int i = 0; i < new_depth; i++)
    {
        Matrix new_matrix = CreateMatrix(new_width / 2, new_height / 2);
        int idx_x = 0;
        for (int j = 1; j < new_height; j += 2)
        {
            int idx_y = 0;
            for (int k = 0; k < new_width; k += 2)
            {
                new_matrix.matrix[idx_x][idx_y] = m.matrix3d[i].matrix[j][k];
                idx_y += 1;
            }
            idx_x += 1;
        }
        output3.matrix3d[i] = new_matrix;
    }
    for (int i = 0; i < new_depth; i++)
    {
        Matrix new_matrix = CreateMatrix(new_width / 2, new_height / 2);
        int idx_x = 0;
        for (int j = 1; j < new_height; j += 2)
        {
            int idx_y = 0;
            for (int k = 1; k < new_width; k += 2)
            {
                new_matrix.matrix[idx_x][idx_y] = m.matrix3d[i].matrix[j][k];
                idx_y += 1;
            }
            idx_x += 1;
        }
        output4.matrix3d[i] = new_matrix;
    }
    Matrix4d output_total = CreateMatrix4d(4, new_depth, new_width / 2, new_height / 2);
    output_total.matrix4d[0] = output1;
    output_total.matrix4d[1] = output2;
    output_total.matrix4d[2] = output3;
    output_total.matrix4d[3] = output4;
    return output_total;
}
#endif
