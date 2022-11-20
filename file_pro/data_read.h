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

str_Matrix read_file(string &file_path)
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
	cout<<count_rows<<endl;
	cout<<count_col+1<<endl;
	str_Matrix data_ma = CreateStr_Ma(1666,1666);	
	ifstream infile2;
	infile2.open(file_path);
	int next_flag = 0;
	while(!infile2.eof())
	{
	infile2 >> data;
	string base = "";
	string added ="";
	string added2 = "";
	string base2 = "";
	int iSize = data.size();
	int flag_if = 1;
	int count_times = 0;
	int count_times2 = 0;
	for(int i = 0;i < iSize; i++)
	{
		//cout<<data[i]<<endl;
 		if(data[i]!=',')
		{
		added+=data[i];		
		}
		else if(data[i]==',')
		{	
			count_times+=1;			
			base = added; 
			char *result = (char*)base.data();
			//cout<<result<<endl;
			data_ma.str_matrix[next_flag][count_times-1] = result;
			added ="";
			base = "";
		}
			if(count_times==count_col)
		{
		added2+=data[i+1];
		base2 = added2;
		char *result2 = (char*)base2.data();
		//cout<<result2<<endl;
		data_ma.str_matrix[next_flag][count_times] = result2;
		}
	}
	next_flag+=1;	
	}
	return data_ma;
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
	Matrix data_ma = CreateMatrix(count_rows,count_col+1);	
	ifstream infile2;
	infile2.open(file_path);
	int next_flag = 0;
	while(!infile2.eof())
	{
	infile2 >> data;
	string base = "";
	string added ="";
	string added2 = "";
	string base2 = "";
	int iSize = data.size();
	int flag_if = 1;
	int count_times = 0;
	int count_times2 = 0;
	for(int i = 0;i < iSize; i++)
	{
		if(data[i]!=',')
		{
		added+=data[i];		
		}
		else if(data[i]==',')
		{	
			count_times+=1;			
			base = added; 
			char *result = (char*)base.data();
			data_ma.matrix[next_flag][count_times-1] = str2double(result);
			added ="";
			base = "";
		}
			if(count_times==count_col)
		{
		added2+=data[i+1];
		base2 = added2;
		char *result2 = (char*)base2.data();
		data_ma.matrix[next_flag][count_times] = str2double(result2);
		}
	}
	next_flag+=1;	
	}
	return data_ma;
}
int save_txt(Matrix mid1,string path = "./",string delimiter = ",",string header="./")
{
	int index_x,index_y;
	ofstream fout(path,ios::app);
	fout<<header<<endl;		
	for(index_x=0;index_x<mid1.row;index_x++)
	{
		for(index_y=0;index_y<mid1.col;index_y++)
		{
			fout<<mid1.matrix[index_x][index_y]<<delimiter;
		}
		fout<<endl;
	}
	fout.close();
	return 0;
}
/*
int read_file(string path = "./",string delimiter = ",",string header = "./")
{
ystring list_word[]=[];
string *p = list_word;
for(int i = 0;i<5;i++)
{
	*(list_word+i) = 1;
	cout<<"*p("<<i<<"): ";
	cout<<*(p+i)<<endl;
}
}
*/
//-----------split------------
//
//
