#ifndef MATRIX_DEF
#define MATRIX_DEF
#pragma once
#include<iostream>
#include<sys/malloc.h>
#include<stdio.h>
#include<string>
using namespace std;
//CreateMatrix Create a Matrix for ones;
//ex:
//2*3
//0 0 0
//0 0 0
//change the value in matrix
typedef struct
{
	int row,col;
	float **matrix;
}Matrix;
Matrix CreateMatrix(int ro,int co)
{
	Matrix m;
	int row,col;
	float **inputMatrix;
	inputMatrix=(float**)malloc(ro*sizeof(float*));
	for(int i = 0;i<ro;i++)
	{
		inputMatrix[i]=(float*)malloc(co*sizeof(float));
	}
	for(int i=0; i<ro; i++)
    {
        for(int j=0; j<co; j++)
        {
            inputMatrix[i][j] = 0;
        }
    }
	m.col = co;
	m.row = ro;
	m.matrix = inputMatrix;
	return m;
}
int change_va(Matrix Matrix ,int index_x,int index_y,float value)
{
	Matrix.matrix[index_x][index_y] = value;
	return 0;
}
#endif
