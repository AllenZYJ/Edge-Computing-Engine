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
	Node u=2, p=3.2,tt = 10,ll = u*p;
    Node f = 1/(1+exp(ll));
    std::cout << "f(x,y) = 1/1+exp(2*3)"<<f<< std::endl;
    std::cout << "∂f/∂u = " << f.gradient(ll) << std::endl;
    std::cout << "∂f/∂p = " << f.gradient(p) << std::endl;
	cout<<"------------autodiff for neraul network-----------"<<endl;
	Matrix data1 = CreateRandMat(2,1);
	Matrix label_data = CreateMatrix(2,1);
	change_va(label_data,0,0,1);
	change_va(label_data,1,0,1);
	cout_mat(data1);
	cout<<"------------data1----------------"<<endl;	
	Matrix we1 = CreateRandMat(2,2);
	cout_mat(we1);	
	cout<<"------------weight-----------"<<endl;	
	edge_layer layer(2,2,we1,label_data,"tanh");
	Matrix grad_before1 = ones(2,2);	
	Matrix out_put = layer.forward(data1,we1);
	Matrix grad_before2 = layer.backward(out_put,label_data,0.1,grad_before1);
	Matrix grad_we1 = CreateRandMat(2,2);
	we1 = subtract(we1,grad_we1);
	cout<<"------------first backward---------"<<endl;	
	edge_layer layer_2(layer.output_shape,2,we1,label_data,"tanh");	
	Matrix out_put2 = layer_2.forward(out_put,we1);
	layer_2.backward(out_put2,label_data,0.1,grad_before2);
	return 0;
}

