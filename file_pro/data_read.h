#include"../matrix/matrix_pro.h"
#include"../matrix/matrix_def.h"
#include<iostream>
#include<fstream>
#include<string>
#include<typeinfo>
#include<cstring>
using namespace std;
string data;
double str2double(char *src)
{
    double ret = 0,sign = 1;
    char *p = src;
    if(*p == '+'){sign = 1;p ++;}
    else if(*p == '-'){sign = -1; p ++; }
    while(*p && (*p != '.'))
    {
        ret*=10;
        ret += (*p) - '0';
        p++;
    }
    if(*p == '.')
    {
        double step = 0.1;
        p++; 
        while(*p)
        {
            ret+=step*((*p)-'0');
            step/=10;
            p++;
        }
    }
    return ret*sign;
}

Matrix read_csv(string &file_path)
{
	int count_rows = 0,count_col = 0;
	ifstream infile;
	infile.open(file_path);
	while(!infile.eof())
	{
	count_col = 0;
	count_rows+=1;
	infile >> data;
	int iSize = data.size();
	int flag_if = 1;
	for(int i = 0;i < iSize; i++)
	{
		if(data[i]==',')
		{
			count_col++;
		}
	}
	}
	Matrix data_ma = CreateMatrix(count_rows+1,count_col+1);	
	ifstream infile2;
	infile2.open(file_path);
	int next_flag = 0;
	while(!infile2.eof())
	{
	next_flag+=1;		
	infile2 >> data;
	string base = "";
    string added ="";
	int iSize = data.size();
	int flag_if = 1;
	int count_times = 0;
	cout<<iSize<<endl;
	for(int i = 0;i < iSize; i++)
	{
		if(data[i]!=',')
		{
		added2+=data[i];
		added+=data[i];		
		}
		else if(data[i]==',')
		{	
			count_times+=1;			
			base = added; 
			char *result = (char*)base.data();
			cout<<"result: "<<result<<endl;
			data_ma.matrix[next_flag][count_times-1] = str2double(result);
			added ="";
			base = "";
		}
	}
	}
	return data_ma;
}
