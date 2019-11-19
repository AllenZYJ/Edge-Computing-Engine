#ifndef MATRIX_PRO
#define MATRIX_PRO
#include<iostream>
#include<string>
/*
matrix add

flag stand use binary or not default 1

move_ele change the x,y to y,x

get_T :get the matrix transpose
input:Matrix a;
output:Matrix a.transpose

*/
void move_ele(int &ele1, int &ele2)
{
  ele1 ^= ele2 ^= ele1 ^= ele2;
}

int add_by(int a,int b)
{
    int ans;
    while(b)
    {
        ans = a^b;
        b = ((a&b)<<1);
        a = ans;
    }
    return a;
} 

//two matrix add
Matrix add(Matrix mid1,Matrix mid2,int flag=1)
{
	if(mid1.row == mid2.row&&mid1.col == mid2.col)
	{
		for(int x = 0;x<mid1.row;x++)
		{
			for(int y = 0;y<mid1.col;y++)
			{
			if(flag==1)
				mid1.matrix[x][y]=add_by(mid1.matrix[x][y],mid2.matrix[x][y]);
			else
				mid1.matrix[x][y]+=mid2.matrix[x][y];
			}
		}
	}
	else{return mid1;}
	return mid1;
}

//subtract use default

Matrix subtract(Matrix mid1,Matrix mid2)
{
	if(mid1.row == mid2.row&&mid1.col == mid2.col)
	{
		for(int i=0; i<mid1.row; i++)
		{
			for(int j=0; j<mid1.col; j++)
			{
				mid1.matrix[i][j] = mid1.matrix[i][j] -mid2.matrix[i][j];
			}
		}
	}
	else{return mid1;}
    return mid1;
}

Matrix mul(Matrix mid1,Matrix mid2)
{
Matrix mid3 = CreateMatrix(mid1.row,mid2.col);
for(int i = 0;i<mid1.row;i++)
{
	for(int j = 0;j<mid2.col;j++)
	{
		for(int k = 0;k<mid1.col;k++)
		{
			mid3.matrix[i][j]=mid3.matrix[i][j]+mid1.matrix[i][k]*mid2.matrix[k][j];
		}
	}
}
	return mid3;	
}
Matrix times_mat(int times,Matrix mid1)
{
    for(int index_x=0; index_x<mid1.row; index_x++)
    {
        for(int index_y=0; index_y<mid1.col; index_y++)
        {
            mid1.matrix[index_x][index_y] = mid1.matrix[index_x][index_y]*times;
        }
    }
    return mid1;
}

Matrix get_T(Matrix mid1)
{
	Matrix mid2 = CreateMatrix(mid1.col,mid1.row);
	for(int index_x = 0;index_x<mid1.row;index_x++)
	{
		for(int index_y=0;index_y<mid1.col;index_y++)
		{
			mid2.matrix[index_y][index_x]=mid1.matrix[index_x][index_y];
		}
	}
	return mid2;
}
double* flatten(Matrix mid1)
{
	int size = mid1.row*mid1.col;
	double balance[size];
	double *p;
	p = balance;
	for ( int i = 0; i <size; i++ )
	{
		int index_x = i/mid1.col;
		int index_y = i%mid1.col;
		*(p+i) = mid1.matrix[index_x][index_y];
	}
	return balance;
}
Matrix matrix_rs(Matrix mid1,int rs_row,int rs_col)
{

	int size = mid1.row*mid1.col;
	double balance[size];
	double *p;
	p = balance;
	int index_x=0,index_y=0;
	Matrix result = CreateMatrix(rs_row,rs_col);
	for ( int i = 0; i <size; i++ )
	{
		int index_x = i/mid1.col;
		int index_y = i%mid1.col;
		*(p+i) = mid1.matrix[index_x][index_y];
	}

	int added = mid1.row*mid1.col;
	for(int i = 0;i<size;i++)
	{
		if(index_y == rs_col)
		{
			index_y=0;	
			index_x+=1;
		}
		result.matrix[index_x][index_y]=*(p+i);
		index_y++;
	}
	return result;
}
double matrix_sum(Matrix mid1)
{
	double sum = 0;
	for(int index_x = 0;index_x<mid1.row;++index_x)
	{
		for(int index_y=0;index_y<mid1.col;++index_y)
		{
			sum+=mid1.matrix[index_x][index_y];
		}
	}
		return sum;
}
double matrix_mean(Matrix mid1)
{
	
	double ele = mid1.row*mid1.col;
	return matrix_sum(mid1)/ele;
}
Matrix appply(Matrix mid1,Matrix mid2,int axis = 0)
{
	int new_row = mid1.row+mid2.row;
	int new_col = mid1.col+mid2.col;
//	cout<<new_row<<":"<<new_col<<endl;
	Matrix result;
	if(axis ==1)
	{result = CreateMatrix(mid1.row,new_col);}
	else{result = CreateMatrix(new_row,mid2.col);}
	for(int index_x = 0;index_x<mid1.row;++index_x)
	{
		for(int index_y=0;index_y<mid1.col;++index_y)
		{
			result.matrix[index_x][index_y] = mid1.matrix[index_x][index_y];
		}
	}
	if(axis == 1)
	{
	for(int index_row=0;index_row<mid2.row;++index_row)
	{
		for(int index_col = mid1.col;index_col<new_col;++index_col)
		{
			result.matrix[index_row][index_col]= mid2.matrix[index_row][mid2.col-new_col+index_col];
		}
	}
	}
	else
	{
//	cout<<result.row<<result.col<<endl;
	for(int index_row=mid1.row;index_row<new_row;++index_row)
	{
		for(int index_col = 0;index_col<mid2.col;++index_col)
		{
//		cout<<"x: "<<index_row-mid2.row<<" ";
//		cout<<"y: "<<index_col<<endl;	
//		cout<<"x->"<<index_row<<endl;
//		cout<<"y->"<<index_col<<endl;
//		cout<<"aim"<<mid2.matrix[index_row-mid2.row][index_col]<<endl;
		result.matrix[index_row][index_col]= mid2.matrix[index_row-mid2.row][index_col];
//		cout<<"result: "<<result.matrix[index_row][index_col]<<endl;
		
		}
	}
	}
	return result;
}

#endif
