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
	Matrix data_mine = CreateMatrix(100,3);
	Matrix label_mine = CreateRandMat(100,1);
	Matrix data = iloc(data_mine,0,1,0,0);
	cout<<"data: ";
	cout_mat(data);
	Matrix label = iloc(label_mine,0,3,0,0);
	cout<<"label: ";
	cout_mat(label);
	Matrix bias1 = CreateRandMat(2,1);
	Matrix weight1 = CreateRandMat(2,data.col);
	cout<<"weight1: ";
	cout_mat(weight1);
	int index = 1;
	Matrix backward2 = CreateMatrix(2,1);
	Matrix before_backward2 = CreateMatrix(2,1);
	change_va(before_backward2,1,0,1);
	for(index = 0;index<99;index++){
	Matrix output1 = sequaltial.forward(get_T(get_row(data_mine,index)),weight1,bias1);
	Matrix output1_without_act = sequaltial.forward_without_act(get_T(get_row(data_mine,index)),weight1,bias1);
	cout<<"output1_without_act: ";
	cout_mat(output1_without_act);
	Matrix weight2 = CreateRandMat(2,output1.row);
	cout<<"weight2: "<<endl;
	cout_mat(weight2);
	Matrix bias2 = CreateRandMat(weight2.row,1);
	Matrix output2 = sequaltial.forward(output1,weight2,bias2);
	Matrix output2_without_act = sequaltial.forward_without_act(output1,weight2,bias2);
	cout<<"output2_without_act: ";
	cout_mat(output2_without_act);
	cout<<"output2: ";
	cout_mat(output2);
	cout<<"output1:"<<endl;
	cout_mat(output1);
	Matrix weight3 = CreateRandMat(3,output2.row);
	Matrix bias3 = CreateRandMat(weight3.row,1);
	Matrix output3 = sequaltial.forward(output2,weight3,bias3);
	Matrix output3_without_act = sequaltial.forward_without_act(output2,weight3,bias3);
	cout<<"row: "<<label.row<<"col: "<<label.col;
	Matrix output_end = sequaltial.end_layer_backward(label,output3_without_act,*loss,*act);
	cout<<"outputend: ";
	cout_mat(output_end);
	cout<<"output3_without_act: ";
	cout_mat(output3_without_act);
	cout<<"output3: ";
	cout_mat(output3);
	Matrix backward3 = sequaltial.backward(output_end,output2_without_act,get_T(weight3),*act);
	cout<<"backward_before: ";
	cout_mat(before_backward2);
	before_backward2 = add(before_backward2,backward2);
	backward2 = sequaltial.backward(backward3,output1_without_act,get_T(weight2),*act);
	cout<<"backward2: ";
	save_txt(backward2,"a.csv");
	cout_mat(before_backward2);
	cout_mat(backward2);
	//can change the before_backward2 values;
	}
}

