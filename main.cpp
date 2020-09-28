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

#include<iostream>
#include<ctime>
#include<string>
#include <time.h>
#include <math.h>
#include <fstream>
#include "./autodiff/node.h"
#include"./matrix/matrix_def.h"
#include"./matrix/matrix_pro.h"
#include"./welcome/score_wel.cpp"
#include"./logistic/logistic_def.h"
#include"./file_pro/data_read.h"
#include"./grad_edge/matrix_grad.h"
using namespace std;
clock_t start, stop;
double duration;
Node z=1;
Node t1 = 1,a13 = 1;
/*自定义的损失函数*/
Node loss_act(Node t1,Node a13)
{
	 Node loss =  0.5*(pow((t1-a13),2));
	 return loss;
}
Node sigmoid_act(Node z)
{
Node sigmoid_act = 1/(1+(1/exp(z)));
return sigmoid_act;
}
//loss
Node (*loss)(Node,Node) = loss_act;
Node (*act)(Node) = sigmoid_act;
int main()
{
	Matrix data_1 = ones(1,3);
	Matrix data_2 = ones(3,1);
	cout_mat(mul(data_1,data_2));
	welcome();
	//begin 
	cout<<"---------autodiff for neraul network-----------"<<endl;
	Matrix data_mine = ones(3,3);
	cout<<"data mine"<<endl;
	cout_mat(data_mine);
	cout<<"data mine"<<endl;
	Matrix label = CreateRandMat(3,1);
	Matrix weight1 = CreateRandMat(3,3);
	Matrix bais1 = ones(3,1);
	Matrix weight2 = CreateRandMat(3,3);
	Matrix bais2 = ones(3,1);
	for(int epoch = 0;epoch<10;epoch++)
	{
	cout<<"---------epoch: "<<epoch<<"------------"<<endl;
	cout_mat(weight1);
	int input_dim = 3;
	int output_dim = 3;
	edge_network sequaltial(input_dim,output_dim);
	Matrix output1 = sequaltial.forward(data_mine,weight1,bais1);
	Matrix output1_without_act = sequaltial.forward_without_act(data_mine,weight1,bais1);
	Matrix output2 = sequaltial.forward(output1,weight2,bais2);
	Matrix output2_without_act = sequaltial.forward_without_act(output1,weight2,bais2);	
	Matrix output_end = sequaltial.end_layer_backward(label,output2_without_act,*loss,*act);
	Matrix backward3 = sequaltial.backward(output_end,output1_without_act,weight2,*act);
	Matrix weight_2_grad = mul(output_end,get_T(output1));
	Matrix weight_1_grad = mul(backward3,get_T(data_mine));
	weight1 = subtract(weight1,times_mat(0.001,weight_1_grad));
	bais1 = subtract(bais1,times_mat(0.001,backward3));
	weight2 = subtract(weight2,times_mat(0.001,weight_2_grad));
	bais2 = subtract(bais2,times_mat(0.001,output_end));
	cout<<"neraul end;"<<endl;
	}
		return 0;
}
