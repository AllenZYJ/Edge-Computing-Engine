using namespace std;
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
*/
double get_sigmoid(double temp)
{
//	cout<<"temp"<<temp<<endl;
	return 1/1+exp(0-temp);
}
Matrix e_sigmoid(Matrix mid1)
{
	Matrix result = CreateMatrix(mid1.row,mid1.col);
	for(int index_x = 0;index_x<mid1.row;index_x++)
	{
		for(int index_y=0;index_y<mid1.col;index_y++)
		{
//			cout<<"ex:"<<get_sigmoid(mid1.matrix[index_x][index_y])<<endl;
			result.matrix[index_x][index_y]= 1.0/(1+exp(-mid1.matrix[index_x][index_y]));
		}
	}
	return result;
}

