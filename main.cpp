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
	string path = "./data.csv";	
	Matrix test_head_mat = CreateMatrix(5,4);
	cout_mat(iloc(read_csv(path),2,199,0,2));
	cout<<"-----------split line-----------"<<endl;	
	Matrix data = read_csv(path);
//	cout_mat(data);
	cout<<"-----------split line-----------"<<endl;
	Matrix a = CreateMatrix(4,3);
	double a_ = e_sigmoid(1);	
	cout<<a_<<endl;
	change_va(a,1,2,1);
	change_va(a,0,1,7);
	change_va(a,0,2,2);
	change_va(a,1,1,3);
	change_va(a,3,2,11);	
	
	start = clock();

	Matrix applyed_ma = appply(a,a,1);
	for(int index_x = 0;index_x<applyed_ma.row;index_x++)
	{
		cout<<"|";
		for(int index_y=0;index_y<applyed_ma.col;index_y++)
		{
			cout<<applyed_ma.matrix[index_x][index_y]<<"|";
		}
		cout<<endl;
	}
	cout<<"--------split---------"<<endl;
	Matrix b = get_T(a);
	for(int index_x = 0;index_x<b.row;index_x++)
	{
		for(int index_y=0;index_y<b.col;index_y++)
		{
			cout<<b.matrix[index_x][index_y]<<" ";
		}
		cout<<endl;
	}
	cout<<"-----------split line-----------"<<endl;
	cout<<"matrix*n"<<endl;
	Matrix c = times_mat(8,b);
	for(int index_x = 0;index_x<c.row;index_x++)
	{
		for(int index_y=0;index_y<c.col;index_y++)
		{
			cout<<c.matrix[index_x][index_y]<<" ";
		}
		cout<<endl;
	}
	cout<<"-----------split line-----------"<<endl;
	cout<<"matrix*matrix"<<endl; 
	Matrix d =mul(c,get_T(c));
	for(int index_x = 0;index_x<d.row;index_x++)
	{
		for(int index_y=0;index_y<d.col;index_y++)
		{
			cout<<d.matrix[index_x][index_y]<<" ";
		}
		cout<<endl;
	}
	cout<<"-----------split line-----------"<<endl;
	Matrix rs = matrix_rs(a,6,5);
	for(int index_x = 0;index_x<rs.row;index_x++)
	{
		for(int index_y=0;index_y<rs.col;index_y++)
		{
			cout<<rs.matrix[index_x][index_y]<<" ";
		}
		cout<<endl;
	}
	times_mat(5,a);
	stop = clock();
    printf("%f\n", (double)(stop - start) / CLOCKS_PER_SEC);
	return 0;
}

