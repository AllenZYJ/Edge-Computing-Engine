#ifndef MATRIX_PRO
#define MATRIX_PRO
#include<iostream>
#include<stdlib.h>
#include<string>
#define random(x) (rand()%x)
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
	Matrix result_subtract = CreateMatrix(mid1.row,mid2.col);
	for(int i=0; i<mid1.row; i++)
	{
		for(int j=0; j<mid1.col; j++)
		{
			//cout<<" mid1.matrix[i][j]"<< mid1.matrix[i][j]<<"mid2.matrix[i][j]"<<mid2.matrix[i][j]<<endl;

			result_subtract.matrix[i][j] = mid1.matrix[i][j]-mid2.matrix[i][j];
		}
	}
    return result_subtract;
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
Matrix times_mat(double times,Matrix mid1)
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
	Matrix result;
	if(axis ==1)
	{result = CreateMatrix(mid1.row,new_col);}
	else{result = CreateMatrix(new_row+1,mid2.col);}
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
	for(int index_row=mid1.row;index_row<new_row;++index_row)
	{
		for(int index_col = 0;index_col<mid2.col;++index_col)
		{
		result.matrix[index_row][index_col]= mid2.matrix[index_row-mid2.row][index_col];
		}
	}
	}
	return result;
}
Matrix head(Matrix mid1)
{
	int col = mid1.col;
	Matrix mid_return = CreateMatrix(6,mid1.col);
	for(int index_x = 0;index_x<6;++index_x)
	{	
		for(int index_y=0;index_y<col;++index_y)
		{
		mid_return.matrix[index_x][index_y] = mid1.matrix[index_x][index_y];
		}
	}
	return mid_return;
}
void cout_mat(Matrix mid1)
{
	for(int index_x = 0;index_x<mid1.row;index_x++)
	{
		for(int index_y=0;index_y<mid1.col;index_y++)
		{
			cout<<mid1.matrix[index_x][index_y]<<",";
		}
		cout<<endl;
	}
}
//iloc [x1,x2,y1,y2]
Matrix iloc(Matrix mid1,int start_x=0,int end_x=0,int start_y=0,int end_y=0)
{
	if(end_y == 0)
	{
		end_y = mid1.col;
	}
	else if(end_x == 0)
	{
		end_x = mid1.row;
	}
	cout<<mid1.row<<end_x<<" "<<start_x<<" "<<end_y<<" "<<start_y<<endl;
	int new_row = end_x-start_x;
	int new_col = end_y-start_y;
//	cout<<"nc"<<new_col<<" nr"<<new_row<<endl;
	Matrix mid_return = CreateMatrix(new_row,new_col);
	for(int index_x = start_x;index_x<end_x;++index_x)
	{	
		for(int index_y=start_y;index_y<end_y;++index_y)
		{
		//	cout<<"start: "<<index_x<<",end: "<<index_y<<endl;
		//	cout<<"re: "<<mid1.matrix[index_x][index_y]<<endl;
		mid_return.matrix[index_x-start_x][index_y-start_y] = mid1.matrix[index_x][index_y];
		}
	}
	return mid_return;
}
Matrix mul_simple(Matrix mid1,Matrix mid2)
{
	if(mid1.row != mid2.row||mid1.col != mid2.col)
	{
		cout<<"Error: shape A&B"<<endl;
		return mid1;
	}
	Matrix result = CreateMatrix(mid1.row,mid1.col);
	for(int index_x = 0;index_x<mid1.row;index_x++)
	{
		for(int index_y=0;index_y<mid1.col;index_y++)
		{
		result.matrix[index_x][index_y] = mid1.matrix[index_x][index_y]*mid2.matrix[index_x][index_y];
		}
	}
	return result;
}
Matrix CreateRandMat(int x_dim,int y_dim)
{
	int index_x,index_y;
	Matrix result = CreateMatrix(x_dim,y_dim);
	for(index_x = 0;index_x<x_dim;++index_x)
	{
		for(index_y=0;index_y<y_dim;++index_y)
		{
			double temp_val = (rand()%30000)*0.0001-1;
			change_va(result,index_x,index_y,temp_val);
		}
	}
return result;
}
double edge_relu(double val_relu)
{
	if(val_relu>0)
	{return val_relu;}
	else
	{return 0;}
}
Matrix mat_relu(Matrix mid1)
{
	Matrix relu_mat = CreateMatrix(mid1.row,mid1.col);
	for(int index_x = 0;index_x<mid1.row;++index_x)
		for(int index_y = 0;index_y<mid1.row;++index_y)
		{
			{
			relu_mat.matrix[index_x][index_y] = edge_relu(mid1.matrix[index_x][index_y]);

			}
		}
	return relu_mat;
}
Matrix mat_sq_loss(Matrix mid1,Matrix mid2)
{
	int index_x,index_y;
	Matrix mat_sq = CreateMatrix(mid1.row,1);
	for(index_x = 0;index_x<mid1.row;++index_x)
	{
		mat_sq.matrix[index_x][0] = pow(mid1.matrix[index_x][0]-mid2.matrix[index_x][0],2);
	}
	return mat_sq;
}
Matrix padding(Matrix mid1,int shape1,int shape2)
{
	Matrix result = CreateMatrix(shape1,shape2);
    for(int index_x = 0;index_x<shape1;index_x++)
	{
		for(int index_y = 0;index_y<shape2;index_y++)
		{
			result.matrix[index_x][index_y] = mid1.matrix[index_x][0];
		}
	}
	return result;
}
#endif
