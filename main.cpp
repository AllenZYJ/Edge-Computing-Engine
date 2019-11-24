#include<iostream>
#include<ctime>
#include<string>
#include <time.h>
#include <math.h>
#include <fstream>
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
	string path = "./new_data2.csv";
	Matrix data = read_csv(path);
	Matrix bais = CreateMatrix(data.row,1);		
	data = appply(data,bais,1);
	Matrix y = iloc(data,0,0,3,4);
	Matrix x_1 = iloc(data,0,0,0,3);
	Matrix x_2 = get_T(x_1);
	double alpha = 0.002;
	int max_epoch = 10000;
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
	return 0;
}

