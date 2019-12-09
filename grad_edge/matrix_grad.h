#include<iostream>
#include<ctime>
#include<string>
#include <time.h>
#include <math.h>
#include <fstream>
#include"./autodiff/node.h"
#include"../matrix/matrix_def.h"
#include"../matrix/matrix_pro.h"
using namespace std;
float sigmoid(float x){
    return 1/(1+exp(x));
}
Node sigmoid(Node z){
   Node sigmoid = 1/(1+(1/exp(z)));
    return sigmoid;
}
struct edge_network
{
	edge_network(int input, int num_neuron){			
    }
	Matrix forward(Matrix data,Matrix weights,Matrix bais)
	{
	//	cout<<"data: ";
	//	cout_mat(data);
	//	cout<<"weights: "<<endl;
	//	cout_mat(weights);	
		Matrix output = mul(get_T(weights),data);		
		output = add(output,bais,0);
		Matrix output1 = e_sigmoid(output);
	//	cout<<"----------forward data--------"<<endl;
	//	cout_mat(data);
	//	cout<<"---------weight1---------"<<endl;
	//	cout_mat(weights);
	//	cout<<"---------bais----------"<<endl;
	//	cout_mat(bais);
	//	cout<<"---------output--------"<<endl;
	//	cout_mat(output1);		
		return output1;
	}
	Matrix forward_without_act(Matrix data,Matrix weights,Matrix bais)
	{
		Matrix output = mul(get_T(weights),data);		
		output = add(output,bais,0);
	//	Matrix output1 = e_sigmoid(output);
	//	cout<<"----------forward data--------"<<endl;
	//	cout_mat(data);
	//	cout<<"---------weight1---------"<<endl;
	//	cout_mat(weights);
	//	cout<<"---------bais----------"<<endl;
	//	cout_mat(bais);
	//	cout<<"---------output--------"<<endl;
	//	cout_mat(output);		
		return output;
	}
	Matrix backward(Matrix grad_next, Matrix output_before,Matrix weights,Node p_(Node))
	{
	//	cout<<"-----------backward-------------"<<endl;
	//	cout_mat(grad_next);
	//	cout<<"-----------grad_next------------"<<endl;
	//	cout_mat(output_before);
	//	cout<<"-----------output before--------"<<endl;
	//	cout_mat(get_T(weights));
	//	cout<<"-----------TTTTTweights--------------"<<endl;
	//	cout_mat(padding(mul_simple(mul(get_T(weights),grad_next),output_before),2,2));
//		cout<<"[[[[[";
		cout<<"grad_next:"<<endl;
		cout_mat(grad_next);
		cout<<"weights"<<endl;
		cout_mat(weights);
		for(int index = 0;index<output_before.row;index++)
		{
			Node z = output_before.matrix[index][0];
//			Node sigmoid =  1/(1+(1/exp(z)));
			Node anyone = p_(z);
			change_va(output_before,index,0,anyone.gradient(z));
			cout<<"grad_edge: "<<anyone.gradient(z)<<endl;
		}
		cout<<"output_before"<<endl;
		cout_mat(output_before);
		cout<<"mul(weights,grad_next)"<<endl;
		cout_mat(mul(weights,grad_next));
		cout<<"grad_backward: "<<endl;
		cout_mat(mul_simple(mul(weights,grad_next),output_before));
		return mul_simple(mul(weights,grad_next),output_before);
	}
	Matrix end_layer_backward(Matrix label,Matrix acti_val,Node loss_fun(Node,Node),Node act_fun(Node))
	{
		Matrix loss_act = CreateMatrix(acti_val.row,acti_val.col);
		Matrix act_output = CreateMatrix(acti_val.row,acti_val.col);
		int index_x,index_y;
		for(index_x=0;index_x<loss_act.row;index_x++)
		{
		//	cout<<"index: "<<index_x;
		Node t1 = label.matrix[index_x][0],z31 =acti_val.matrix[index_x][0];
	//	cout<<"t1: "<<t1<<endl;
		Node a13 = 1/(1+(1/exp(z31)));
		Node loss = loss_fun(t1,a13);
		Node act = act_fun(z31);		
		act_output.matrix[index_x][0] = act.gradient(z31);
		loss_act.matrix[index_x][0] = loss.gradient(a13);
		cout<<"z31: "<<z31<<endl;
		cout<<"z31_grad: "<<act.gradient(z31)<<endl;
		cout<<"a13: "<<a13<<"t1: "<<t1<<endl;
		cout<<"loss: "<<loss<<endl;
		cout<<"a13_gradient: "<<loss.gradient(a13)<<endl;
		}
		Matrix mid_grad_end = mul_simple(loss_act,act_output);
		cout<<"mid_grad_end";
		cout_mat(mid_grad_end);
		return mid_grad_end;
	}
};

