#include"../matrix/matrix_pro.h"
#include"../matrix/matrix_def.h"
#include<iostream>
#include<fstream>
#include<string>
#include<typeinfo>
using namespace std;
string data;	
void read_csv(string &file_path)
{
	
	int count_rows = 0;
	ifstream infile;
	infile.open(file_path);
	while(!infile.eof())
	{
	count_rows+=1;
	infile >> data;
	cout<<data<<count_rows<<endl;
	}
}
