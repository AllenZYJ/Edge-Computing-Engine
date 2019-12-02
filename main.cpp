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
int main()
{/*
	welcome();	
	string path = "./data/new_data2.csv";
	Matrix data = read_csv(path);
	Matrix bais = CreateMatrix(data.row,1);		
	data = appply(data,bais,1);
	Matrix y = iloc(data,0,0,3,4);
	Matrix x_1 = iloc(data,0,0,0,3);
	Matrix x_2 = get_T(x_1);
	double alpha = 0.002;
	int max_epoch = 1;
	Matrix weight = CreateMatrix(3,1);
	change_va(weight,0,0,1);
	change_va(weight,1,0,1);
	change_va(weight,2,0,1);
	int epoch = 0;
	for(epoch = 0;epoch<=max_epoch;epoch++)
	{
	cout<<"-----------split-line-----------"<<endl;			
	Matrix temp_mul = mul(x_1,weight);
	Matrix h =e_sigmoid(temp_mul);
	Matrix error = subtract(y,h);
	Matrix temp_update = mul(x_2,error);
	Matrix updata = add(weight,times_mat(alpha,temp_update),0);
	cout_mat(weight);
	cout<<"epoch: "<<epoch<<" error: "<<matrix_sum(error)<<endl;
	cout<<"-----------split-line-----------"<<endl;	
	}
	stop = clock();
    printf("%f\n", (double)(stop - start) / CLOCKS_PER_SEC);
	*/
	cout<<"------------autodiff for neraul network-----------"<<endl;
	Matrix data_mine = CreateMatrix(2,1);
	change_va(data_mine,0,0,0.55);
	change_va(data_mine,1,0,0.2);
	cout<<"data mine"<<endl;
	cout_mat(data_mine);
	cout<<"data mine"<<endl;
	Matrix label = CreateRandMat(2,1);
	change_va(label,0,0,0.4);
	change_va(label,1,0,0.8);
	Matrix weight1 = CreateRandMat(2,2);
	change_va(weight1,0,0,0.1);
	change_va(weight1,0,1,0.2);
	change_va(weight1,1,0,0.2);
	change_va(weight1,1,1,0.4);
	cout<<"weight1";
	cout_mat(weight1);
	cout<<"weight1";
	Matrix bais1 = ones(2,1);
	Matrix weight2 = CreateRandMat(2,2);
	change_va(weight2,0,0,0.5);
	change_va(weight2,1,0,0.6);
	change_va(weight2,0,1,0.7);
	change_va(weight2,1,1,0.8);
	Matrix bais2 = ones(2,1);	
	Matrix weight3 = CreateRandMat(2,2);
	Matrix bais3 = ones(2,1);	
	Matrix weight4 = CreateRandMat(2,2);
	Matrix bais4 = ones(2,1);	
	for(int epoch = 0;epoch<1000;epoch++)
	{
	cout<<"---------epoch: "<<epoch<<"------------"<<endl;
//	cout_mat(weight1);
	int input_dim = 2;
	int output_dim = 2;
	edge_network sequaltial(input_dim,output_dim);
	Matrix output1 = sequaltial.forward(data_mine,weight1,bais1);
	//output1 acti_val;
	Matrix output1_without_act = sequaltial.forward_without_act(data_mine,weight1,bais1);
	//output1_without_act without activation;
	Matrix output2 = sequaltial.forward(output1,weight2,bais2);
	Matrix output2_without_act = sequaltial.forward_without_act(output1,weight2,bais2);	
	Matrix output_end = sequaltial.end_layer_backward(label,output2_without_act);
	//last output layer should feed the value without activation;
	//output_end : the gradient of loss layer;
	Matrix backward3 = sequaltial.backward(output_end,output1_without_act,weight2);
	//backward3: the gradient of the behind layer;
	Matrix weight_2_grad = mul(output_end,get_T(output1));
	Matrix weight_1_grad = mul(backward3,get_T(data_mine));
	weight1 = subtract(weight1,times_mat(0.001,weight_1_grad));
	bais1 = subtract(bais1,times_mat(0.001,backward3));
	weight2 = subtract(weight2,times_mat(0.001,weight_2_grad));
	bais2 = subtract(bais2,times_mat(0.001,output_end));
	}
	
	return 0;
}

