#include<iostream>
#include<time.h>
#include<string>
#include<math.h>
#include<fstream>
#include"./autodiff/node.h"
#include"./matrix/matrix_def.h"
#include"./matrix/matrix_pro.h"
#include"./welcome/score_wel.cpp"
#include"./logistic/logistic_def.h"
#include"./file_pro/data_read.h"
#include"./grad_edge/matrix_grad.h"
using namespace std;
Node z = 1;
Node t1 = 1,a13 = 1;
Node loss_act(Node t1,Node a13)
{
	Node loss = 0.5*(pow((t1-a13),2));
	return loss;
}
Node sigmoid_act(Node z)
{
	Node sigmoid_act = 1/(1+(1/exp(z)));
	return sigmoid_act;
}
Node (*loss)(Node,Node) = loss_act;
Node (*act)(Node) = sigmoid_act;
int main()
{
	cout<<"begin to train"<<endl;
	int input_dim = 2;
	int output_dim = 2;
	edge_network sequaltial(input_dim,output_dim);
	Matrix data_mine = CreateMatrix(100,2);
	Matrix label_mine = CreateRandMat(100,1);
	Matrix data = iloc(data_mine,0,1,0,0);
	cout<<"data: ";
	cout_mat(data);
	Matrix label = iloc(label_mine,0,1,0,0);
	cout<<"label: ";
	cout_mat(label);
	Matrix bias1 = CreateRandMat(2,1);
	Matrix weight1 = CreateRandMat(2,data.col);
	cout<<"weight1: ";
	cout_mat(weight1);
	int index = 0;
	Matrix output1 = sequaltial.forward(get_T(get_row(data_mine,index)),weight1,bias1);
	Matrix weight2 = CreateRandMat(output1.row,2);
	cout<<"weight2: "<<endl;
	cout_mat(weight2);
	Matrix bias2 = CreateRandMat(weight2.row,1);
	Matrix output2 = sequaltial.forward(output1,weight2,bias2);
	cout_mat(output2);
	cout<<"output1:"<<endl;
	cout_mat(output1);

}

