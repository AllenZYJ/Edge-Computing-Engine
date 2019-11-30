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
Node mean_square_error(Matrix mid1,Matrix mid2){
    Node loss;
    for(size_t i=0 ; i<mid2.row ; i++){
        loss += pow(mid1.matrix[i]-mid2.matrix[i], 2);
    }
    return loss;
}
struct edge_network
{
	edge_network(int input, int output){
    }
	Matrix forward(Matrix data,Matrix weights)
	{
	//	cout<<"data: ";
	//	cout_mat(data);
	//	cout<<"weights: "<<endl;
	//	cout_mat(weights);
		Matrix output = mul(weights,data);
		output = e_sigmoid(output);
	//	cout<<"forward output: ";
	//	cout_mat(output);
	//	cout<<"----------forward finish--------"<<endl;
		return output;
	}
	Matrix backward(Matrix grad_next, Matrix output_before,Matrix weights)
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
		return mul_simple(mul(get_T(weights),grad_next),output_before);

	}
	Matrix end_layer_backward(Matrix label,Matrix acti_val)
	{
		Matrix loss_act = CreateMatrix(acti_val.row,acti_val.col);
		Matrix act_output = CreateMatrix(acti_val.row,acti_val.col);
		int index_x,index_y;
		for(index_x=0;index_x<loss_act.row;index_x++)
		{
		Node t1 = label.matrix[index_x][0],z31 =acti_val.matrix[index_x][0];
		Node a13 = 1+exp(z31);
		Node loss = 0.5*(pow((t1-a13),2));
		Node act = 1/(1+exp(z31));
		act_output.matrix[index_x][0] = act.gradient(z31);
		loss_act.matrix[index_x][0] = loss.gradient(a13);
	//	cout<<"z31: "<<z31<<endl;
	//	cout<<"z31_grad: "<<act.gradient(z31)<<endl;
	//	cout<<"a13: "<<a13<<"t1: "<<t1<<endl;
		cout<<"loss: "<<loss<<endl;	
	//	cout<<"gradient: "<<loss.gradient(a13)<<endl;
		}		
		Matrix mid_grad_end = mul_simple(loss_act,act_output);
	//	cout<<"[[[[[[[[";
	//	cout_mat(mid_grad_end);
		return mid_grad_end;
	}
};

