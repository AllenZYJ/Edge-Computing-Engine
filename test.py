import re
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from numpy import exp
def data():#读取txt文本内的数据并转成需要的list
    f=open('data2.txt')
    data=[]
    for each_line in f:
        each_line=each_line.strip('\n')
        res=re.split(" ",each_line)
        res=list(filter(None,res))
        res= list(map(float,res))
        data.append(res)
    for i in range(len(data)):#加一个偏执项
        data[i].insert(0,1)
    f.close()
    print(len(data[1]))
    return data
def sin_cos(data,weights):#画图
    x0=[]
    x1=[]
    y0=[]
    y1=[]
    for i in data:
        if(i[3]==0):
            x0.append(i[1])
            y0.append(i[2])
        else:
            x1.append(i[1])
            y1.append(i[2])
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(x0,y0,marker='+',label='1',s=40,c='r')
    ax.scatter(x1,y1,marker='*',label='1',s=40,c='b')
    x = np.arange(-3, 3, 0.1)
    y = (-weights[0, 0] - weights[1, 0] * x) / weights[2, 0]  #matix
    ax.plot(x, y)
    plt.xlabel('X1')
    plt.ylabel('X2')
    plt.savefig('test.png')
    #plt.show()
def sigmoid(inX):#逻辑函数
        return 1.0/(1+exp(-inX))
def tiduxiajiang_sigmoid(data):
    x=[]
    y=[]
    for i in range(len(data)):
        x.append(data[i][:-1])
        y.append(data[i][-1:])
    alpha=0.002
    maxcycle=1
    x=np.mat(x)#转成矩阵
    y=np.mat(y)
    x2=x.T#矩阵转逆
    weights=np.mat([[1],[1],[1]])
    for i in range(maxcycle):
        h = sigmoid(x* weights)
        print(h.shape)
        error=y-h
        print("error.shape: ",error.shape)
        print(error)
        print("x2.h",x2.shape)
        print(x2)
        a = x2*error
        print("alpha*x2*error: ",a.shape)
        weights=weights+alpha*x2*error
        print(sum(error))#看是否收敛
    return weights
data=data()
weights=tiduxiajiang_sigmoid(data)
sin_cos(data,weights)
# print('\n')
#print(weights)
#to_csv(data)