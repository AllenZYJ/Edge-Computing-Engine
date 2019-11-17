#include<iostream>
#include<ctime>
#include<string>
#include <time.h>
#include <math.h>
clock_t start, stop;    //clock_t为clock()返回的变量类型
double duration;
#include"./welcome/score_wel.cpp"
#include"./matrix/matrix_def.cpp"
#include"./matrix/matrix_pro.cpp"
using namespace std;
int main()
{
   	welcome();
	Matrix a = CreateMatrix(3,4);
	Matrix b = CreateMatrix(3,11);
	cout<<"end"<<endl;
	change_va(a,1,2,1);
	change_va(a,0,0,9);
	change_va(a,0,2,2);
	change_va(a,1,1,8);
	start = clock();
	Matrix rs = matrix_rs(a,2,6);
	cout<<matrix_mean(a)<<endl;
/*	add(a,b,1);
	mul(a,b);
	for(int index_x = 0;index_x<a.row;index_x++)
	{
		for(int index_y=0;index_y<a.col;index_y++)
		{
			cout<<a.matrix[index_x][index_y]<<" ";
		}
		cout<<endl;
	}
	cout<<"------------"<<endl;
	Matrix a_ = get_T(a);
	*/
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

