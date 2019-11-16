#include <iostream>
#include <sys/malloc.h>
#include <stdio.h>
using namespace std;

typedef struct
{
    int row,col;
    float **matrix;
} Matrix;

typedef struct
{
    char *name;
    char *number;
} Student;

Matrix CreateMatrix()
{
    Matrix m;
    int row,col;
    cout << "输入行数与列数：" << endl;
    cin >> row >> col;
    float **enterMatrix ;
    enterMatrix=(float**) malloc(row*sizeof(float*)) ;
    for(int i=0; i<row; i++)
        enterMatrix[i] = (float *)malloc(col * sizeof(float));
    cout<<"输入你的矩阵："<<endl;
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            cin >> enterMatrix[i][j];
        }
    }
    m.col = col;
    m.row = row;
    m.matrix = enterMatrix;
    return m;
}

//初始化一个行为row列为col矩阵
Matrix InitMatrix(int row,int col)
{
    Matrix m;
    float **matrix ;
    matrix=(float**) malloc(row*sizeof(float*)) ;
    for(int i=0; i<row; i++)
        matrix[i] = (float *)malloc(col * sizeof(float));
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            matrix[i][j] = 0;
        }
    }
    m.col = col;
    m.row = row;
    m.matrix = matrix;
    return m;
}

Matrix add(Matrix m1, Matrix m2)
{
    for(int i=0; i<m1.row; i++)
    {
        for(int j=0; j<m1.col; j++)
        {
            m1.matrix[i][j] = m1.matrix[i][j] +m2.matrix[i][j];
        }
    }
    return m1;
}

Matrix sub(Matrix m1, Matrix m2)
{
    for(int i=0; i<m1.row; i++)
    {
        for(int j=0; j<m1.col; j++)
        {
            m1.matrix[i][j] = m1.matrix[i][j] -m2.matrix[i][j];
        }
    }
    return m1;
}

int calRowCol(Matrix M1,Matrix M2,int row,int col)//row为M1的行 col为m2的列
{
    int result = 0;
    int same = M1.col;
    for(int j=0; j<same; j++)
    {
        result+=M1.matrix[row][j]*M2.matrix[j][col];
    }

    return result;
}


Matrix Mul(Matrix m1, Matrix m2)
{
    Matrix result = InitMatrix(m1.row,m2.col);
    for(int i=0; i<m1.row; i++)
    {
        for(int j=0; j<m2.col; j++)
        {
            result.matrix[i][j] = calRowCol(m1,m2,i,j);
        }
    }
    return result;
}

Matrix numMul(Matrix m, int num)
{
    cout<<"数值:"<<num<<endl;
    for(int i=0; i<m.row; i++)
    {
        for(int j=0; j<m.col; j++)
        {
            m.matrix[i][j] = m.matrix[i][j]*num;
        }
    }
    return m;
}

Matrix printMatrix(Matrix m)
{
    for(int i=0; i<m.row; i++)
    {
        for(int j=0; j<m.col; j++)
        {
            cout << m.matrix[i][j] << "  ";
        }
        cout<<endl;
    }
}

int main()
{
    int num = 0;
    do
    {
        cin>>num;
        if(1 == num|| 2 == num || 3 == num)
        {
            cout<<"请输入矩阵1"<<endl;
            Matrix m1 = CreateMatrix();
            cout<<"请输入矩阵2"<<endl;
            Matrix m2 = CreateMatrix();
            cout<<"两矩阵为"<<endl;
            printMatrix(m1);
            cout<<endl;
            printMatrix(m2);
            switch(num)
            {
            case 1:
            {
                if(m1.col!=m2.col || m1.row!=m2.row)
                {
                    cout<<"行列不同"<<endl;
                }
                else{
                    cout<<"结果为："<<endl;
                    printMatrix(add(m1,m2));
                }
                break;
            }
            case 2:
            {

                if(m1.col!=m2.col || m1.row!=m2.row)
                {
                    cout<<"参数错误"<<endl;
                }
                else{
                    cout<<"结果为："<<endl;
                    printMatrix(sub(m1,m2));
                }
                break;

            }
            case 3:
            {
                if(m1.col!=m2.row)
                {
                    cout<<"参数错误"<<endl;
                }
                else{
                    cout<<"结果为："<<endl;
                    printMatrix(Mul(m1,m2));
                }
                break;
            }
            default:
                break;
            }
        }
        else if(4 == num)
        {
            int number = 1;
            cout<<"请输入矩阵"<<endl;
            Matrix m = CreateMatrix();
            cout<<"请输入数值"<<endl;
            cin>>number;
            cout<<"矩阵为："<<endl;
            printMatrix(m);
            cout<<"数值为："<<endl;
            cout<<number<<endl;
            printMatrix(numMul(m,number));
        }
        cout<<"按回车继续....";

        getchar();
        getchar();
        system("cls");
    }
    while(1 == num|| 2 == num || 3 == num ||4 == num);
    return 0;
}
