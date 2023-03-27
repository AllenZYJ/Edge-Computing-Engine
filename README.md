

<!-- <div align=center><img src="./picture/01.svg"/></div> -->
<div align=center><img src="picture/image-20200418210521131.png" alt="image-20200418210521131"  /></div>

<div align=center><h1>Edge-Engine</h1></div>
<div align=center>一个开源的科学计算引擎


Email: zk@likedge.top

[![Edge support](https://img.shields.io/badge/SUPPORT-Macos-brightgreen)](https://support.apple.com/downloads/macos)
[![Edge support](https://img.shields.io/badge/SUPPORT-Ubuntu-brightgreen)](https://ubuntu.com/download/server)
[![Edge passing](https://img.shields.io/badge/Edge--CI-passing-blue)](https://github.com/AllenZYJ/Edge-Computing-Engine/actions)
[![GitHub license](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](https://raw.githubusercontent.com/tesseract-ocr/tesseract/master/LICENSE)



</div>


------



> 项目开始日期 : 2019/10/01
>
> 测试 : main.cpp | nerual_network.cpp | 
>
> 2022年11月29日14:56:59：卷积层和bn层的批量前向
>
> 2022年11月19日12:28:29：实现卷积神经网络单元前向传播
>
> 2019年10月01日12:28:56：新增全连接神经网络架构(新增全连接网络正向传播和反向传播的测试demo)
>
> 测试环境:
>
> MacBook Pro、ubuntu
>
> 编译器环境:
>
> macos:
> Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/c++/4.2.1
> Apple LLVM version 10.0.1 (clang-1001.0.46.4)
> Target: x86_64-apple-darwin18.7.0
> Thread model: posix
> version
> ubuntu:
> g++ (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0
> Copyright (C) 2019 Free Software Foundation, Inc.
> This is free software; see the source for copying conditions.  There is NO
> warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.




------

这是什么?

![path](./picture/path.png)

## 快速开始

```
git clone git@github.com:AllenZYJ/Edge-Computing-Engine.git

cd to install_diff
```

进入install_diff目录:

执行

```
make
make install
```

编译demo入口程序

```shell
g++ main.cpp -o main -lautodiff
```

或者BP测试程序

```shell
g++ nerual_network.cpp -o main
```

运行

```shell
./main
```

## 算子系列

卷积：

```c++
double conv_test(Matrix mid1,int input_dim = 3,int output_channels = 3,int stride = 1,int kernel_size = 2,int mode = 0,int padding = 0)
```

测试：

```c
g++ conv_test.cpp -o conv_test -lautodiff && ./conv_test
```
## Opencv c++
```c
cd to opencv4.x.x/build
cmake  ../opencv-4.x
cmake --build .

g++ conv_test.cpp -o ma -lautodiff -I /usr/local/include/opencv4 -L/usr/local/lib -lopencv_imgcodecs -lopencv_core
```

### 模型定义方法:

```c
edge_network(int input, int num_neuron)
```

作为序列模型api

edge_network作为一个类型存在,位于matrix_grad.h中结构体类型的数据

定义了前向传播函数,前向传播无激活版,反向传播,末层反向传播,四大最常用的函数主体.

完整的序列模型:

![image-20200128154352842](picture/image-20200128154352842.png)

### 实现5层全连接层,可自定义神经元和激活函数,损失函数

全连接层使用方法:

第一层的权重自定义,而后调用forward函数前向传播一层,自动求出激活以后的值,激活函数可自定义.

首先定义一个权重矩阵和偏置矩阵,第一个矩阵的维度大小使用数据列去定义:

```c
Matrix bias1 = CreateRandMat(2,1);
Matrix weight1 = CreateRandMat(2,data.col);
```

之后可以输出第一层前向传播的值,同时可以定义下一层的bias的维度, row使用第一层的权重矩阵的行,第二层的权重矩阵的行使用了第一层的输出的行, 而列自行定义即可, 这一点体现了前向传播算法的维度相容. 也就是:

```c
Matrix output1 = sequaltial.forward(get_T(get_row(data_mine,index)),weight1,bias1);
```

```c
Matrix weight2 = CreateRandMat(output1.row,2);
Matrix bias2 = CreateRandMat(weight2.row,1);
Matrix output2 = sequaltial.forward(output1,weight2,bias2);
```

同时第二层的输出也可以求出来,以此类推 .

最终输出代码见nerual_test.cpp ![nerual_test1](./picture/nerual_test1.png)

代码:

```c
Matrix data_mine = CreateRandMat(2,1);
Matrix label = CreateMatrix(2,1);
Matrix weight1 = CreateRandMat(2,2);
Matrix weight2 = CreateRandMat(2,2);
Matrix weight3 = CreateRandMat(2,2);
Matrix weight4 = CreateRandMat(2,2);
for(int epoch = 0;epoch<20;epoch++)
{
cout_mat(weight1);
edge_network sequaltial(2,2);

Matrix output1 = sequaltial.forward(data_mine,weight1);
Matrix output2 = sequaltial.forward(output1,weight2);
Matrix output3 = sequaltial.forward(output2,weight3);
Matrix output4 = sequaltial.forward(output3,weight4);
Matrix output_end = sequaltial.end_layer_backward(label,output4);
//get the forward
Matrix backward1 = sequaltial.backward(output_end,output3,weight4);
Matrix grad_w1w2 = mul_simple(backward1,data_mine);
Matrix backward2 = sequaltial.backward(backward1,output2,weight3);
Matrix grad_w3w4 = mul_simple(backward2,data_mine);
Matrix backward3 = sequaltial.backward(backward2,output1,weight2);
Matrix grad_w5w6 = mul_simple(backward3,data_mine);
Matrix backward4 = sequaltial.backward(backward3,output4,weight1);
Matrix grad_w7w8 = mul_simple(backward4,data_mine);
weight1 = subtract(weight1,times_mat(0.0001,padding(grad_w1w2,2,2)));
weight2 = subtract(weight2,times_mat(0.0001,padding(grad_w3w4,2,2)));
weight3 = subtract(weight3,times_mat(0.0001,padding(grad_w5w6,2,2)));
weight4 = subtract(weight4,times_mat(0.0001,padding(grad_w7w8,2,2)));
}
```
```shell
---------epoch: 0------------
loss: 4.65667
loss: 3.28273
---------epoch: 1------------
loss: 4.65655
loss: 3.28265
---------epoch: 2------------
loss: 4.65643
loss: 3.28257
---------epoch: 3------------
loss: 4.65631
loss: 3.28249
---------epoch: 4------------
loss: 4.65619
loss: 3.2824
---------epoch: 5------------
loss: 4.65607
loss: 3.28232
---------epoch: 6------------
loss: 4.65596
loss: 3.28224
---------epoch: 7------------
loss: 4.65584
loss: 3.28216
---------epoch: 8------------
loss: 4.65572
loss: 3.28208
---------epoch: 9------------
loss: 4.6556
loss: 3.282
---------epoch: 10------------
loss: 4.65548
loss: 3.28192
---------epoch: 11------------
loss: 4.65536
loss: 3.28184
---------epoch: 12------------
loss: 4.65524
loss: 3.28176
---------epoch: 13------------
loss: 4.65512
loss: 3.28168
---------epoch: 14------------
loss: 4.65501
loss: 3.2816
---------epoch: 15------------
loss: 4.65489
loss: 3.28152
---------epoch: 16------------
loss: 4.65477
loss: 3.28144
---------epoch: 17------------
loss: 4.65465
loss: 3.28136
---------epoch: 18------------
loss: 4.65453
loss: 3.28128
---------epoch: 19------------
loss: 4.65441
loss: 3.2812
```

## Bp反向传播的demo程序基于Pytorch官方代码模拟实现测试

迭代结果 :

W1: 0.6944 1.52368
	-1.46644 -0.154097
W2: 1.10079
	0.462984
loss: 0.559269

epoch:100 , 可自行测试.

输出最终损失和参数迭代结果.

-----------split-line-----------
2.79955
0.36431
-0.451694
epoch: 100 error: 6.05895
-----------split-line-----------
0.009167(sum of loss)

### 目前实现的程序接口

### API:

- [x] Matrix read_csv(string &file_path)读取格式化文件(csv),返回一个自动计算长度的矩阵.

- [x] 实现格式化文件写入接口.比较pandas.to_csv.

- [x] 矩阵广播机制,实现padding接口

- [x] 全连接层前向传播和反向传播接口,支持自动求导

- [x] 矩阵微分和自动求导接口封装

- [x] int save_txt(Matrix mid1,string path = "./",string delimiter = ",",string header="./") 设计文件流获取文件头部接口 , 写入格式化文件 , 已设计支持矩阵类型数据写入,支持自定义表头,写入文件路径 , 自定义分隔符,默认为" , ".

- [x] Create a matrix :  create(row,cols)开辟一个矩阵结构的内存,元素初值为0;

- [x] Change the element for matrix void move_ele(int &ele1, int &ele2),修改某一个位置的元素的值.

- [x] Matrix1+Matrix2 : Matrix add(Matrix mid1,Matrix mid2,int flag=1),矩阵加和操作接口,可选位运算加速.

- [x] Flag is how to compete the ele ,default 1 ,bitwise operation(位运算加速).

- [x] Matrix1-Matrix2 : Matrix subtract(Matrix mid1,Matrix mid2)

- [x] Matrix1*Matrix2 : Matrix mul(Matrix mid1,Matrix mid2)

- [x] Matrix1*n : Matrix times_mat(int times,Matrix mid1)

- [x] Matrix1's Transposition : Matrix get_T(Matrix mid1)矩阵转置

- [x] Mul(matrix1,matrix2)矩阵乘积(完整数学定义).

- [x] double* flatten(Matrix mid1) : Return a flattened array.矩阵展开

- [x] Matrix matrix_rs(Matrix mid1,int rs_row,int rs_col) 矩阵的结构压缩

- [x] double matrix_sum(Matrix mid1)矩阵求和

- [x] double matrix_mean(Matrix mid1)均值

- [x] Matrix appply(Matrix mid1,Matrix mid2,int axis = 0)矩阵拼接

- [x] Matrix iloc(Matrix mid1,int start_x=0,int end_x=0,int start_y=0,int end_y=0)矩阵切片

- [x] Matrix mul_simple(Matrix mid1,Matrix mid2)为了贴合机器学习的需要,实现了矩阵对应元素相乘,请与传统意义的矩阵乘法区分开.

- [x] Relu激活函数矩阵接口

- [x] 均方误差矩阵接口

- [x] 创建随机权重矩阵接口

- [x] 卷积神经网络定义(包括但不限于卷积核,池化层定义,自定义损失接口).

  ### 即将着手开发:

- [ ] 主流网络架构实现.

  

## 反向传播测试demo:

```c
#include<iostream>
#include<ctime>
#include<string>
#include<time.h>
#include<math.h>
#include<fstream>
#include<stdlib.h>
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
	string path = "./data/nerual_data.csv";
	Matrix data = read_csv(path);
	Matrix bais = CreateMatrix(data.row,1);		
	Matrix x = iloc(data,0,100,0,2);
	Matrix y = iloc(data,0,100,2,3);
	int N=100,in_Dim=2,H_num=2,out_Dim=2;
	double learning_rate = 0.0001;
	Matrix W1 = CreateRandMat(in_Dim,H_num);
	Matrix W2 = CreateRandMat(H_num,out_Dim);
	cout_mat(W1);
	cout_mat(W2);
	for(int epoch = 0;epoch<100;epoch++)
	{
		Matrix x_w1 = mul(x,W1);
		Matrix re = mat_relu(x_w1);
		Matrix out = mul(re,W2);
		Matrix mat_sq = mat_sq_loss(out,y);
		Matrix grad_y_pred = times_mat(2.0,subtract(out,y));
		Matrix grad_w2 = mul(get_T(re),grad_y_pred);
		Matrix grad_h_relu = mul(grad_y_pred,get_T(W2));
		Matrix grad_h_relu_copy = mat_relu(grad_h_relu);
		Matrix grad_w1 = mul(get_T(x),grad_h_relu_copy);
		Matrix dw1 = times_mat(learning_rate,mul(get_T(x),grad_h_relu_copy));
		W1 = subtract(W1,dw1);
		W2 = subtract(W2,times_mat(learning_rate,grad_w2));
		cout<<"W1: ";
		cout_mat(W1);
		cout<<"W2: ";
		cout_mat(W2);
		cout<<"loss"<<": ";
		cout<<matrix_sum(mat_sq)/100<<endl;
	}
}
```



## 演示:矩阵乘法

Matrix **A**：

| 第1列   | 第2列   | 第3列   | 第4列   | 第5列   |
| ------- | ------- | ------- | ------- | ------- |
| 72.0000 | 0.0000  | 0.0000  | 0.0000  | 0.0000  |
| 0.0000  | 64.0000 | 0.0000  | 0.0000  | 0.0000  |
| 16.0000 | 8.0000  | 0.0000  | 0.0000  | 0.0000  |
| 0.0000  | 0.0000  | 56.0000 | 16.0000 | 32.0000 |
| 0.0000  | 0.0000  | 0.0000  | 0.0000  | 0.0000  |
| 0.0000  | 0.0000  | 0.0000  | 0.0000  | 0.0000  |

MAtrix **B**：

| 第1列   | 第2列   | 第3列   | 第4列   | 第5列  | 第6列  |
| ------- | ------- | ------- | ------- | ------ | ------ |
| 72.0000 | 0.0000  | 16.0000 | 0.0000  | 0.0000 | 0.0000 |
| 0.0000  | 64.0000 | 8.0000  | 0.0000  | 0.0000 | 0.0000 |
| 0.0000  | 0.0000  | 0.0000  | 56.0000 | 0.0000 | 0.0000 |
| 0.0000  | 0.0000  | 0.0000  | 16.0000 | 0.0000 | 0.0000 |
| 0.0000  | 0.0000  | 0.0000  | 32.0000 | 0.0000 | 0.0000 |

To

| 第1列     | 第2列     | 第3列     | 第4列     | 第5列  | 第6列  |
| --------- | --------- | --------- | --------- | ------ | ------ |
| 5184.0000 | 0.0000    | 1152.0000 | 0.0000    | 0.0000 | 0.0000 |
| 0.0000    | 4096.0000 | 512.0000  | 0.0000    | 0.0000 | 0.0000 |
| 1152.0000 | 512.0000  | 320.0000  | 0.0000    | 0.0000 | 0.0000 |
| 0.0000    | 0.0000    | 0.0000    | 4416.0000 | 0.0000 | 0.0000 |
| 0.0000    | 0.0000    | 0.0000    | 0.0000    | 0.0000 | 0.0000 |
| 0.0000    | 0.0000    | 0.0000    | 0.0000    | 0.0000 | 0.0000 |

## 演示: 矩阵展开(flatten).

double* flatten(Matrix mid1)

|  1   |  2   |  3   |
| :--: | :--: | :--: |
|  2   |  4   |  6   |
|  7   |  8   |  9   |

​	To

| 1    | 2    | 3    | 2    | 4    | 6    | 7    | 8    | 9                  |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | :----------------- |
|      |      |      |      |      |      |      |      | Like numpy.flatten |

function:

## 演示: 邻接矩阵的参数定义:

​	Matrix appply(Matrix mid1,Matrix mid2,int axis = 0)

> 参数 axis=0 :

| 0    | 7    | 2    |
| ---- | ---- | ---- |
| 0    | 3    | 1    |
| 0    | 0    | 0    |
| 0    | 0    | 11   |
| 0    | 7    | 2    |
| 0    | 3    | 1    |
| 0    | 0    | 0    |
| 0    | 0    | 11   |
------

> axis = 1:

| 0    | 7    | 2    | 0    | 7    | 2    |
| ---- | ---- | ---- | ---- | ---- | ---- |
| 0    | 3    | 1    | 0    | 3    | 1    |
| 0    | 0    | 0    | 0    | 0    | 0    |
| 0    | 0    | 11   | 0    | 0    | 11   |

------
## 矩阵 padding

mid1.wid: 10, kernel_size: 3, stride: 3, padding_wid: 2
mid1.wid: 10, kernel_size: 3, stride: 3, padding_wid: 2, padding_high: 2
|     |      |      |      |      |      |      |      |      |      |      |      |
| --- | ---  | ---  | ---  | ---  | ---  | ---  | ---  | ---  | ---  | ---  | ---  |
| 0   | 0    | 0    | 0    | 0    | 0    | 0    | 0    | 0    | 0    | 0    | 0    |
| 0   | -0.626600 | 0.810600 | 1.072900 | 1.111000 | 0.556900 | 0.345700 | -0.279100 | 1.001200 | 1.785300 | -0.735700 | 0    |
| 0   | -0.789300 | -0.351300 | 1.811300 | -0.780600 | -0.837700 | -0.005400 | -0.094100 | 0.142000 | -0.815500 | 1.186100 | 0    |
| 0   | 1.871900 | -0.898500 | -0.276100 | -0.095300 | -0.142100 | 0.833700 | 0.358200 | -0.870000 | -0.418200 | 1.558500 | 0    |
| 0   | 1.105900 | 1.706800 | 1.549700 | -0.569300 | 1.530000 | -0.671800 | 1.189500 | 0.043700 | 1.302500 | 0.733100 | 0    |
| 0   | -0.319000 | 0.202600 | 0.285000 | 1.103900 | -0.987300 | 0.493700 | 0.932100 | -0.595000 | 0.517200 | 1.225800 | 0    |
| 0   | 1.394400 | 0.119300 | 1.536400 | -0.789700 | 0.590100 | 0.737300 | 0.522400 | -0.959800 | 1.327900 | -0.910700 | 0    |
| 0   | -0.081900 | -0.224100 | -0.379300 | -0.525000 | 1.770800 | 0.864100 | 0.646200 | -0.966900 | 0.592900 | 0.614600 | 0    |
| 0   | -0.780800 | -0.057800 | 0.655500 | 0.275800 | 1.418900 | 1.220800 | 1.505400 | 0.230500 | -0.657900 | 1.346900 | 0    |
| 0   | -0.149900 | 1.788800 | -0.663400 | 0.835300 | 0.475900 | 1.826600 | -0.112600 | -0.412000 | -0.624900 | 0.412000 | 0    |

## 更新2019/11/18/00:12

- [x] read_csv 通过文件流读取逗号分隔符文件,返回一个自动计算长度的矩阵.

  例如 CSV's head :

  | -0.017612 | 14.053064 | 0    |
  | --------- | --------- | ---- |
  | -1.395634 | 4.662541  | 1    |
  | -0.752157 | 6.53862   | 0    |
  | -1.322371 | 7.152853  | 0    |
  | 0.423363  | 11.054677 | 0    |
  | 0.406704  | 7.067335  | 1    |

  Get:

  ![](./picture/WX20191119-105411@2x.png)

  

  ## Logistic Regression demo base Edge:


```c
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
	string path = "./new_data2.csv";
	Matrix data = read_csv(path);
	Matrix bais = CreateMatrix(data.row,1);		
	data = appply(data,bais,1);
	Matrix y = iloc(data,0,0,3,4);
	Matrix x_1 = iloc(data,0,0,0,3);
	Matrix x_2 = get_T(x_1);
	double alpha = 0.002;
	int max_epoch = 100;
	Matrix weight = CreateMatrix(3,1);
	change_va(weight,0,0,1);
	change_va(weight,1,0,1);
	change_va(weight,2,0,1);
	int epoch = 0;
	for(epoch = 0;epoch<=max_epoch;epoch++)
	{
	cout<<"-----------split-line-----------"<<endl;			
	Matrix temp_mul = mul(x_1,weight);
	Matrix h =e_sigmoid(temp_mul);
	Matrix error = subtract(y,h);
	Matrix temp_update = mul(x_2,error);
	Matrix updata = add(weight,times_mat(alpha,temp_update),0);
	cout_mat(weight);
	cout<<"epoch: "<<epoch<<" error: "<<matrix_sum(error)<<endl;
	cout<<"-----------split-line-----------"<<endl;	
	}
	stop = clock();
  printf("%f\n", (double)(stop - start) / CLOCKS_PER_SEC);
	return 0;
}
```
Something :

> 1. 矩阵元素默认为1
> 2. 使用位运算加速防止填充过大的数值,但是会损失一定精度,慎用.
> 3. 记得delete(matrix)在你使用完一个矩阵计算单元以后.
> 4. api接口更多的接近于pandas和numpy的使用习惯.
> 5. 更多的细节参见目前最新的代码
> 6. 欢迎star和关注.
> 7. autodiff部分感谢国外博主Omar的思路提醒.
>

------



<div align = center><img src = './picture/星月.svg'></div>


 个人小站:[极度空间](http://likedge.top/)

作者邮箱:zk@likedge.top | edge@ibooker.org.cn

 QQ:2533524298
