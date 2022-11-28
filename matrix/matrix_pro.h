/*
███████╗██████╗  ██████╗ ███████╗                ███████╗███╗   ██╗ ██████╗ ██╗███╗   ██╗███████╗
██╔════╝██╔══██╗██╔════╝ ██╔════╝                ██╔════╝████╗  ██║██╔════╝ ██║████╗  ██║██╔════╝
█████╗  ██║  ██║██║  ███╗█████╗                  █████╗  ██╔██╗ ██║██║  ███╗██║██╔██╗ ██║█████╗
██╔══╝  ██║  ██║██║   ██║██╔══╝                  ██╔══╝  ██║╚██╗██║██║   ██║██║██║╚██╗██║██╔══╝
███████╗██████╔╝╚██████╔╝███████╗                ███████╗██║ ╚████║╚██████╔╝██║██║ ╚████║███████╗
╚══════╝╚═════╝  ╚═════╝ ╚══════╝                ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚═╝  ╚═══╝╚══════╝
Author：Edge
Web：likedge.top
Date：20200925

matrix add
flag stand use binary or not default 1

move_ele change the x,y to y,x

get_T :get the matrix transpose
input:Matrix a;
output:Matrix a.transpose

and -- just test
---------------------------------------------------------------
if you have the better answer on it , it is nothing, just test~
---------------------------------------------------------------

*/
#ifndef MATRIX_PRO
#define MATRIX_PRO
#define random(x) (rand() % x)
void move_ele(int &ele1, int &ele2)
{
	ele1 ^= ele2 ^= ele1 ^= ele2;
}

int add_by(int a, int b)
{
	int ans;
	while (b)
	{
		ans = a ^ b;
		b = ((a & b) << 1);
		a = ans;
	}
	return a;
}

// two matrix add
Matrix add(Matrix mid1, Matrix mid2, int flag = 1)
{
	if (mid1.row == mid2.row && mid1.col == mid2.col)
	{
		for (int x = 0; x < mid1.row; x++)
		{
			for (int y = 0; y < mid1.col; y++)
			{
				if (flag == 1)
					mid1.matrix[x][y] = add_by(mid1.matrix[x][y], mid2.matrix[x][y]);
				else
					mid1.matrix[x][y] += mid2.matrix[x][y];
			}
		}
	}
	else
	{
		return mid1;
	}
	return mid1;
}
// Matrix3d cat(Matrix mid1,Matrix mid2,int flag=1)
// {
// 	CreateMatrix3d(1, 3, 2);
// }
// subtract use default

Matrix subtract(Matrix mid1, Matrix mid2)
{
	Matrix result_subtract = CreateMatrix(mid1.row, mid2.col);
	for (int i = 0; i < mid1.row; i++)
	{
		for (int j = 0; j < mid1.col; j++)
		{
			result_subtract.matrix[i][j] = mid1.matrix[i][j] - mid2.matrix[i][j];
		}
	}
	return result_subtract;
}
Matrix subtract_ele(Matrix mid1, float to_subtract)
{
	Matrix result_subtract = ones(mid1.row, mid1.col);
	for (int i = 0; i < mid1.row; i++)
	{
		for (int j = 0; j < mid1.col; j++)
		{
			result_subtract.matrix[i][j] = mid1.matrix[i][j] - to_subtract;
		}
	}
	return result_subtract;
}
Matrix add_ele(Matrix mid1, float to_add)
{
	Matrix result_add = ones(mid1.row, mid1.col);
	for (int i = 0; i < mid1.row; i++)
	{
		for (int j = 0; j < mid1.col; j++)
		{
			result_add.matrix[i][j] = mid1.matrix[i][j] + to_add;
		}
	}
	return result_add;
}
Matrix matrix_division(Matrix mid1, double todivision)
{
	Matrix result_d = ones(mid1.row, mid1.col);
	for (int i = 0; i < mid1.row; i++)
	{
		for (int j = 0; j < mid1.col; j++)
		{
			// cout << mid1.matrix[i][j] << "/" << todivision << endl;
			result_d.matrix[i][j] = mid1.matrix[i][j] / todivision;
		}
	}
	return result_d;
}
Matrix matrix_power(Matrix mid1)
{
	Matrix result_power = ones(mid1.row, mid1.col);
	for (int i = 0; i < mid1.row; i++)
	{
		for (int j = 0; j < mid1.col; j++)
		{
			result_power.matrix[i][j] = mid1.matrix[i][j] * mid1.matrix[i][j];
		}
	}
	return result_power;
}

Matrix mul(Matrix mid1, Matrix mid2)
{
	Matrix mid3 = CreateMatrix(mid1.row, mid2.col);
	for (int i = 0; i < mid1.row; i++)
	{
		for (int j = 0; j < mid2.col; j++)
		{
			for (int k = 0; k < mid1.col; k++)
			{
				mid3.matrix[i][j] = mid3.matrix[i][j] + mid1.matrix[i][k] * mid2.matrix[k][j];
			}
		}
	}
	return mid3;
}
Matrix times_mat(double times, Matrix mid1)
{
	for (int index_x = 0; index_x < mid1.row; index_x++)
	{
		for (int index_y = 0; index_y < mid1.col; index_y++)
		{
			mid1.matrix[index_x][index_y] = mid1.matrix[index_x][index_y] * times;
		}
	}
	return mid1;
}

Matrix get_T(Matrix mid1)
{
	Matrix mid2 = CreateMatrix(mid1.col, mid1.row);
	for (int index_x = 0; index_x < mid1.row; index_x++)
	{
		for (int index_y = 0; index_y < mid1.col; index_y++)
		{
			mid2.matrix[index_y][index_x] = mid1.matrix[index_x][index_y];
		}
	}
	return mid2;
}

// double* flatten(Matrix mid1)
// {
// 	int size = mid1.row*mid1.col;
// 	double balance[size];
// 	double *p;
// 	p = balance;
// 	for ( int i = 0; i <size; i++ )
// 	{
// 		int index_x = i/mid1.col;
// 		int index_y = i%mid1.col;
// 		*(p+i) = mid1.matrix[index_x][index_y];
// 	}
// 	return balance;
// }

Matrix flatten(Matrix mid1)
{
	// Matrix :[2,3]
	// 		   [2,4]
	//        -> [2,3,2,4]
	int size = mid1.row * mid1.col;
	Matrix mid2 = CreateMatrix(1, mid1.col * mid1.row);
	int count = 0;
	for (int i = 0; i < mid1.row; i++)
	{
		for (int j = 0; j < mid1.col; j++)
		{
			count += 1;
			mid2.matrix[0][count] = mid1.matrix[i][j];
		}
	}
	return mid2;
}

Matrix matrix_rs(Matrix mid1, int rs_row, int rs_col)
{

	int size = mid1.row * mid1.col;
	double balance[size];
	double *p;
	p = balance;
	int index_x = 0, index_y = 0;
	Matrix result = CreateMatrix(rs_row, rs_col);
	for (int i = 0; i < size; i++)
	{
		int index_x = i / mid1.col;
		int index_y = i % mid1.col;
		*(p + i) = mid1.matrix[index_x][index_y];
	}

	int added = mid1.row * mid1.col;
	for (int i = 0; i < size; i++)
	{
		if (index_y == rs_col)
		{
			index_y = 0;
			index_x += 1;
		}
		result.matrix[index_x][index_y] = *(p + i);
		index_y++;
	}
	return result;
}

double matrix_sum(Matrix mid1)
{
	double sum = 0;
	for (int index_x = 0; index_x < mid1.row; ++index_x)
	{
		for (int index_y = 0; index_y < mid1.col; ++index_y)
		{
			sum += mid1.matrix[index_x][index_y];
		}
	}
	return sum;
}
double matrix_mean(Matrix mid1)
{

	double ele = mid1.row * mid1.col;
	return matrix_sum(mid1) / ele;
}
float matrix_var(Matrix mid1)
{
	double ele = mid1.row * mid1.col;
	ele = ele - 1;
	float sum_mid1 = matrix_sum(matrix_power(subtract_ele(mid1, matrix_mean(mid1)))) + 0.000001;
	float var = sum_mid1 / ele;
	return var;
}

Matrix mat_apply(Matrix mid1, Matrix mid2, int axis = 0)
// axis = 1:mat1,mat2 => mat1mat2
// asix = 0:mat1,mat2 => mat1
// 						 mat2
{
	int new_row = mid1.row + mid2.row;
	int new_col = mid1.col + mid2.col;
	Matrix result;
	if (axis == 1)
	{
		result = CreateMatrix(mid1.row, new_col);
	}
	else
	{
		result = CreateMatrix(new_row + 1, mid2.col);
	}
	for (int index_x = 0; index_x < mid1.row; ++index_x)
	{
		for (int index_y = 0; index_y < mid1.col; ++index_y)
		{
			result.matrix[index_x][index_y] = mid1.matrix[index_x][index_y];
		}
	}
	if (axis == 1)
	{
		for (int index_row = 0; index_row < mid2.row; ++index_row)
		{
			for (int index_col = mid1.col; index_col < new_col; ++index_col)
			{
				result.matrix[index_row][index_col] = mid2.matrix[index_row][mid2.col - new_col + index_col];
			}
		}
	}
	else
	{
		for (int index_row = mid1.row; index_row < new_row; ++index_row)
		{
			for (int index_col = 0; index_col < mid2.col; ++index_col)
			{
				result.matrix[index_row][index_col] = mid2.matrix[index_row - mid2.row][index_col];
			}
		}
	}
	return result;
}
Matrix head(Matrix mid1)
{
	int col = mid1.col;
	Matrix mid_return = CreateMatrix(6, mid1.col);
	for (int index_x = 0; index_x < 6; ++index_x)
	{
		for (int index_y = 0; index_y < col; ++index_y)
		{
			mid_return.matrix[index_x][index_y] = mid1.matrix[index_x][index_y];
		}
	}
	return mid_return;
}
void cout_mat(Matrix mid1)
{
	cout << "[" << endl;
	for (int index_x = 0; index_x < mid1.row; index_x++)
	{
		for (int index_y = 0; index_y < mid1.col; index_y++)
		{
			cout << mid1.matrix[index_x][index_y] << ",";
		}
		cout << endl;
	}
	cout << "]" << endl;
}
void cout_mat3d(Matrix3d mat3d)
{

	int i, j, k;
	cout << "-----[" << endl;
	for (i = 0; i < mat3d.dep; i++)
	{
		cout_mat(mat3d.matrix3d[i]);
	}
	cout << "]-----" << endl;
}
// iloc [x1,x2,y1,y2]
Matrix iloc(Matrix mid1, int start_x = 0, int end_x = 0, int start_y = 0, int end_y = 0)
{
	if (end_y == 0)
	{
		end_y = mid1.col;
	}
	else if (end_x == 0)
	{
		end_x = mid1.row;
	}
	// cout<<mid1.row<<end_x<<" "<<start_x<<" "<<end_y<<" "<<start_y<<endl;
	int new_row = end_x - start_x;
	int new_col = end_y - start_y;
	//	cout<<"nc"<<new_col<<" nr"<<new_row<<endl;
	Matrix mid_return = CreateMatrix(new_row, new_col);
	for (int index_x = start_x; index_x < end_x; ++index_x)
	{
		for (int index_y = start_y; index_y < end_y; ++index_y)
		{
			mid_return.matrix[index_x - start_x][index_y - start_y] = mid1.matrix[index_x][index_y];
		}
	}
	return mid_return;
}
Matrix mul_simple(Matrix mid1, Matrix mid2)
{
	if (mid1.row != mid2.row || mid1.col != mid2.col)
	{
		cout << "Error: shape A&B" << endl;
		return mid1;
	}
	Matrix result = CreateMatrix(mid1.row, mid1.col);
	for (int index_x = 0; index_x < mid1.row; index_x++)
	{
		for (int index_y = 0; index_y < mid1.col; index_y++)
		{
			result.matrix[index_x][index_y] = mid1.matrix[index_x][index_y] * mid2.matrix[index_x][index_y];
		}
	}
	return result;
}

double edge_relu(double val_relu)
{
	if (val_relu > 0)
	{
		return val_relu;
	}
	else
	{
		return 0;
	}
}
Matrix mat_relu(Matrix mid1)
{
	Matrix relu_mat = CreateMatrix(mid1.row, mid1.col);
	for (int index_x = 0; index_x < mid1.row; ++index_x)
		for (int index_y = 0; index_y < mid1.row; ++index_y)
		{
			{
				relu_mat.matrix[index_x][index_y] = edge_relu(mid1.matrix[index_x][index_y]);
			}
		}
	return relu_mat;
}
Matrix mat_sq_loss(Matrix mid1, Matrix mid2)
{
	int index_x, index_y;
	Matrix mat_sq = CreateMatrix(mid1.row, 1);
	for (index_x = 0; index_x < mid1.row; ++index_x)
	{
		mat_sq.matrix[index_x][0] = pow(mid1.matrix[index_x][0] - mid2.matrix[index_x][0], 2);
	}
	return mat_sq;
}
Matrix padding(Matrix mid1, int shape1, int shape2)
{
	Matrix result = CreateMatrix(shape1, shape2);
	for (int index_x = 0; index_x < shape1; index_x++)
	{
		for (int index_y = 0; index_y < shape2; index_y++)
		{
			result.matrix[index_x][index_y] = mid1.matrix[index_x][0];
		}
	}
	return result;
}
Matrix get_row(Matrix mid1, int index)
{
	int index_y = 0;
	Matrix mid2 = CreateMatrix(1, mid1.col);
	for (index_y = 0; index_y < mid1.col; index_y++)
	{
		mid2.matrix[0][index_y] = mid1.matrix[index][index_y];
	}
	return mid2;
}
Matrix conv_element(Matrix mid1, Matrix kernel, int kernel_size = 2, int stride = 1)
{
	Matrix conv_result = CreateMatrix(((mid1.row - kernel_size) / stride) + 1, ((mid1.col - kernel_size) / stride) + 1);
	for (int x_ = 0; x_ <= (mid1.row - kernel_size) / stride; x_ += stride)
	{
		for (int y_ = 0; y_ <= (mid1.col - kernel_size) / stride; y_ += stride)
		{
			Matrix crop_pic = iloc(mid1, x_, x_ + kernel.col, y_, y_ + kernel.row);
			change_va(conv_result, x_, y_, matrix_sum(mul_simple(crop_pic, kernel)));
		}
	}
	// cout<<"row: "<<conv_result.row<<" , "<<"col: "<<conv_result.col<<endl;
	// cout_mat(conv_result);
	return conv_result;
}
/*
parameter:
	Matrix mid1,
	int input_dim = 3
	int output_channels = 3
	int stride = 1
	int kernel_size = 2
	int mode = 0
	int padding = 0
*/
// Matrix get_mat2d_mat3d(Matrix3d mi3d1, int get_index)
// {
// 	Matrix result = CreateMatrix(mi3d1.wid, mi3d1.high);
// 	int i, j, k;
// 	for (j = 0; j < mi3d1.wid; j++)
// 	{
// 		for (k = 0; k < mi3d1.high; k++)
// 		{
// 			result.matrix[j][k] = mi3d1.matrix3d[get_index][j][k];
// 		}
// 	}
// 	return result;
// }
Matrix3d conv_test(Matrix3d mid1, int input_dim = 3, int output_channels = 3, int stride = 1, int kernel_size = 2, int mode = 0, int padding = 0)
{
	{
		Matrix output;
		Matrix mid_rgb[input_dim];
		for (int rgb_idx = 0; rgb_idx < input_dim; rgb_idx++)
		{
			mid_rgb[rgb_idx] = mid1.matrix3d[rgb_idx];
		}
		Matrix filters[output_channels][input_dim];
		for (int channel_index = 0; channel_index < input_dim; channel_index++)
		{
			for (int filter_index = 0; filter_index < output_channels; filter_index++)
			{
				Matrix kernel = ones(kernel_size, kernel_size);
				filters[channel_index][filter_index] = kernel;
			}
		}
		if (mode == 0)
		{
			// cout << "input_img:" << endl;
			// for (int i = 0; i < input_dim; i++)
			// {
			// 	// cout << "---------rgb: " << i << "---------" << endl;
			// 	// cout_mat(mid_rgb[i]);
			// }
			Matrix conv_result = CreateMatrix(((mid1.wid - kernel_size) / stride) + 1, ((mid1.high - kernel_size) / stride) + 1);
			Matrix kernel = ones(kernel_size, kernel_size);
			// cout << "--------- kernels: 3x3--------" << endl;
			// cout_mat(kernel);
			// cout << "--------- output: ---------" << endl;
			Matrix feature_maps[output_channels];
			for (int filter_idx = 0; filter_idx < output_channels; filter_idx++)
			{
				Matrix sum_rgb = CreateMatrix(((mid1.wid - kernel_size) / stride) + 1, ((mid1.high - kernel_size) / stride) + 1);
				for (int channel_idx = 0; channel_idx < input_dim; channel_idx++)
				{
					sum_rgb = add(sum_rgb, conv_element(mid_rgb[channel_idx], filters[channel_idx][filter_idx], kernel_size, stride), 0);
					// cout << "sum_rgb"
					//  << "filters_index: " << filter_idx << " " << endl;
					// cout_mat(sum_rgb);
				}
				feature_maps[filter_idx] = sum_rgb;
			}
			Matrix3d output3d = CreateMatrix3d(output_channels, feature_maps[0].row, feature_maps[0].col);
			// for (int i = 0; i < output_channels; i++)
			// {
			// 	output3d.matrix3d[i] = feature_maps[i].matrix;
			// }
			return output3d;
		}
	}
}
#endif
