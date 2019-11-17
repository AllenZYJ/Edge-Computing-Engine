# 矩阵滴乘法运算
# 注意：需要传入np.matrix类型数据
def Matrix_Mul(a,b):
    if a.shape[1] != b.shape[0]:
        print('这两个矩阵无法做乘法，请检查左边矩阵的列数是否与右边矩阵的行数相等！')
    else:
        c = np.zeros(a.shape[0]*b.shape[1]).reshape(a.shape[0],b.shape[1])
        for i in range(a.shape[0]):
            for j in range(b.shape[1]):
                for k in range(a.shape[1]):
                    print(i,k)
                    c[i,j] = c[i,j] + a[i,k]*b[k,j]
    return c

import numpy as np
a = np.matrix([[2,3,4],[1,0,5]])
b =a.T
print(Matrix_Mul(a=b,b=a))
