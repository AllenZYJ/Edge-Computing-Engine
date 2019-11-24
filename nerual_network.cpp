#include<iostream>
#include<ctime>
#include<string>
#include<time.h>
#include<math.h>
#include<fstream>
#include<stdlib.h>
#include"./matrix/matrix_def.h"
#include"./matrix/matrix_pro.h"
#include"./welcome/score_wel.cpp"
#include"./logistic/logistic_def.h"
#include"./file_pro/data_read.h"
using namespace std;
clock_t start, stop;
double duration;
int main()
{
	welcome();	
	string path = "./data/nerual_data.csv";
	Matrix data = read_csv(path);
	Matrix bais = CreateMatrix(data.row,1);		
	Matrix x = iloc(data,0,100,0,2);
	Matrix y = iloc(data,0,100,2,3);
	int N=100,in_Dim=2,H_num=2,out_Dim=2;
	double learning_rate = 0.0001;
	Matrix W1 = CreateRandMat(in_Dim,H_num);
	Matrix W2 = CreateRandMat(H_num,out_Dim);
	cout_mat(W1);
	cout_mat(W2);
	for(int epoch = 0;epoch<100;epoch++)
	{
		Matrix x_w1 = mul(x,W1);
		Matrix re = mat_relu(x_w1);
		Matrix out = mul(re,W2);
		Matrix mat_sq = mat_sq_loss(out,y);
		Matrix grad_y_pred = times_mat(2.0,subtract(out,y));
		Matrix grad_w2 = mul(get_T(re),grad_y_pred);
		Matrix grad_h_relu = mul(grad_y_pred,get_T(W2));
		Matrix grad_h_relu_copy = mat_relu(grad_h_relu);
		Matrix grad_w1 = mul(get_T(x),grad_h_relu_copy);
		Matrix dw1 = times_mat(learning_rate,mul(get_T(x),grad_h_relu_copy));
		W1 = subtract(W1,dw1);
		W2 = subtract(W2,times_mat(learning_rate,grad_w2));
		cout<<"W1: ";
		cout_mat(W1);
		cout<<"W2: ";
		cout_mat(W2);
		cout<<"loss"<<": ";
		cout<<matrix_sum(mat_sq)/100<<endl;
	}
}
