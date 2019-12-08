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
typedef struct
{
	int row,col;
	string **str_matrix;
}str_Matrix;
str_Matrix CreateStr_Ma(int ro, int co)
{
	str_Matrix str_arr;
	int row,col;
	string **designma;
	designma = (string**)malloc(ro*sizeof(string*));
	for(int i = 0;i<ro;i++)
	{
		designma[i] = (string*)malloc(co*sizeof(string));
	}
	for(int i = 0;i <ro;i++)
	{
		for(int j =0;j<co;j++)
		{
			//cout<<"designma"<<designma[i][j]<<endl;
			designma[i][j] = "edge";
		}
	}
	str_arr.row = ro;
	str_arr.col = co;
	str_arr.str_matrix = designma;
	return str_arr;
}
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
Matrix ones(int ro,int co)
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
            inputMatrix[i][j] = 1;
        }
    }
	m.col = co;
	m.row = ro;
	m.matrix = inputMatrix;
	return m;
}

void cout_strmat(str_Matrix mid1)
{
	for(int index_x = 0;index_x<mid1.row;index_x++)
	{
		for(int index_y=0;index_y<mid1.col;index_y++)
		{
			cout<<mid1.str_matrix[index_x][index_y]<<",";
		}
		cout<<endl;
	}
}
int change_va(Matrix Matrix ,int index_x,int index_y,float value)
{
	Matrix.matrix[index_x][index_y] = value;
	return 0;
}

#endif
