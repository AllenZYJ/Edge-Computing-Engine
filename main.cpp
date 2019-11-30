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
{
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
	cout<<"------------autodiff for neraul network-----------"<<endl;
	Matrix data_mine = CreateRandMat(2,1);
	Matrix label = CreateMatrix(2,1);
	Matrix weight1 = CreateRandMat(2,2);
	Matrix weight2 = CreateRandMat(2,2);
	Matrix weight3 = CreateRandMat(2,2);
	Matrix weight4 = CreateRandMat(2,2);
	for(int epoch = 0;epoch<20;epoch++)
	{
		cout<<"---------epoch: "<<epoch<<"------------"<<endl;
//	cout_mat(weight1);
	edge_network sequaltial(2,2);
	Matrix output1 = sequaltial.forward(data_mine,weight1);
	Matrix output2 = sequaltial.forward(output1,weight2);
	Matrix output3 = sequaltial.forward(output2,weight3);
	Matrix output4 = sequaltial.forward(output3,weight4);
	Matrix output_end = sequaltial.end_layer_backward(label,output4);
	//get the forward
	Matrix backward1 = sequaltial.backward(output_end,output3,weight4);
	Matrix grad_w1w2 = mul_simple(backward1,data_mine);
	Matrix backward2 = sequaltial.backward(backward1,output2,weight3);
	Matrix grad_w3w4 = mul_simple(backward2,data_mine);
	Matrix backward3 = sequaltial.backward(backward2,output1,weight2);
	Matrix grad_w5w6 = mul_simple(backward3,data_mine);
	Matrix backward4 = sequaltial.backward(backward3,output4,weight1);
	Matrix grad_w7w8 = mul_simple(backward4,data_mine);
	weight1 = subtract(weight1,times_mat(0.0001,padding(grad_w1w2,2,2)));
	weight2 = subtract(weight2,times_mat(0.0001,padding(grad_w3w4,2,2)));
	weight3 = subtract(weight3,times_mat(0.0001,padding(grad_w5w6,2,2)));
	weight4 = subtract(weight4,times_mat(0.0001,padding(grad_w7w8,2,2)));
	}
	return 0;
}

