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
struct edge_layer
{
	Matrix weights;
	Matrix bias;
	int input_shape;
	int output_shape;
	edge_layer(int input, int output,Matrix weights,Matrix label,string activation){
        this->input_shape = input;
        this->output_shape = output;
		this->weights = weights;
    }
	Matrix forward(Matrix data,Matrix weights)
	{
		cout<<"data: ";
		cout_mat(data);
		cout<<"weights: "<<endl;
		cout_mat(weights);
		Matrix output = mul(weights,data);
	//	output = e_sigmoid(output);
	cout<<"forward output: ";
		cout_mat(output);
		return output;
	}
	Matrix backward(Matrix output,Matrix label,const float& learning_rate,Matrix grad_before)
	{
		Matrix grad = CreateMatrix(weights.row,weights.col);

		for(int index_x=0;index_x<weights.row;index_x++)
			{
				Node target=label.matrix[index_x][0],_y_ = output.matrix[index_x][0];	
				cout<<"target: "<<target<<endl;
				Node loss = target-pow(_y_,2);
				cout<<"loss: "<<loss<<endl;
			for(int index_y = 0;index_y<weights.col;index_y++)
			{			
				grad.matrix[index_x][index_y] = loss.gradient(_y_);
				weights.matrix[index_x][index_y]-=loss.gradient(_y_);
			}
		}
		grad = mul_simple(grad,grad_before);
		cout<<"grad:";
		cout_mat(grad);
	}
};

