# Edge-Computing-Engine

##*Edge : 一个开源的科学计算引擎*

## 0.1 矩阵定义：

​	api:

​		Create a matrix :  create(row,cols)

​		Change the element for matrix void move_ele(int &ele1, int &ele2)

​		Matrix1+Matrix2 : Matrix add(Matrix mid1,Matrix mid2,int flag=1)

​		Flag is how to compete the ele ,default 1 ,bitwise operation.

​		flag标志,默认为1,使用位运算,在亿万级数据下,位运算平均速度优于其他方法,故而做了实现,深入理解.

​		Matrix1-Matrix2 : Matrix subtract(Matrix mid1,Matrix mid2)

​		Matrix1*Matrix2 : Matrix mul(Matrix mid1,Matrix mid2)

​		Matrix1*n : Matrix times_mat(int times,Matrix mid1)

​		Matrix1's Transposition : Matrix get_T(Matrix mid1)

​		double* flatten(Matrix mid1)

|  1   |  2   |  3   |
| :--: | :--: | :--: |
|  2   |  4   |  6   |
|  7   |  8   |  9   |

To

| 1    | 2    | 3    | 2    | 4    | 6    | 7    | 8    | 9    |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
|      |      |      |      |      |      |      |      |      |

Flatten: 



一些细节:

1. 创建矩阵,默认为1^n
2. 动态分配内存，防止矩阵过大
3. 可修改的地方(已完成)
4. 为了能够节省内存后期删除,采用使用指针临时开辟数组空间
5. delete(matrix);

具体见文档,源代码.

## 0.2 矩阵相乘：

1. 查看数值顺序，给出不合理的问题
2. 结果储存接口
3. 矩阵均值,行列式

## 0.3 matrix reshape操作

1. 重新定义矩阵大小，支持flatten和reshape操作
2. 拉伸变化后矩阵数值不变，方式：重定义

## 更新某一个值

1. 利用某个地址赋值方法，更新矩阵的值
2. 实现更新单个或者多个
