#include"../matrix/matrix_def.h"
#include"../matrix/matrix_pro.h"
#include<math.h>
#include<iostream>
using namespace std;
double e_sigmoid(int z)
{
	return 1/(1+exp(z));
}
