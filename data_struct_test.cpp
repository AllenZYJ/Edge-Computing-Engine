/*
███████╗██████╗  ██████╗ ███████╗                ███████╗███╗   ██╗ ██████╗ ██╗███╗   ██╗███████╗
██╔════╝██╔══██╗██╔════╝ ██╔════╝                ██╔════╝████╗  ██║██╔════╝ ██║████╗  ██║██╔════╝
█████╗  ██║  ██║██║  ███╗█████╗                  █████╗  ██╔██╗ ██║██║  ███╗██║██╔██╗ ██║█████╗
██╔══╝  ██║  ██║██║   ██║██╔══╝                  ██╔══╝  ██║╚██╗██║██║   ██║██║██║╚██╗██║██╔══╝
███████╗██████╔╝╚██████╔╝███████╗                ███████╗██║ ╚████║╚██████╔╝██║██║ ╚████║███████╗
╚══════╝╚═════╝  ╚═════╝ ╚══════╝                ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚═╝  ╚═══╝╚══════╝
Author：Edge
Web：likedge.top
Date：20201213
*/

#include <iostream>
#include <ctime>
#include <string>
#include <time.h>
#include <math.h>
#include <fstream>
#include "./autodiff/node.h"
#include "./matrix/matrix_def.h"
#include "./matrix/matrix_pro.h"
#include "./welcome/score_wel.cpp"
#include "./logistic/logistic_def.h"
#include "./file_pro/data_read.h"
#include "./grad_edge/matrix_grad.h"
#include "./data_struct/data_struct_pro.h"
#include "./data_struct/data_struct_pro.c"
using namespace std;
int a[101], n; //定义全局变量，这两个变量需要在子函数中使用
int i, j, t;
// quick_sort
void quicksort(int left, int right)
{
   int i, j, t, temp;
   if (left > right)
      return;

   temp = a[left]; // temp中存的就是基准数
   i = left;
   j = right;
   while (i != j)
   {
      //顺序很重要，要先从右边开始找
      while (a[j] >= temp && i < j)
         j--;
      //再找右边的
      while (a[i] <= temp && i < j)
         i++;
      //交换两个数在数组中的位置
      if (i < j)
      {
         t = a[i];
         a[i] = a[j];
         a[j] = t;
      }
   }
   //最终将基准数归位
   a[left] = a[i];
   a[i] = temp;

   quicksort(left, i - 1);  //继续处理左边的，这里是一个递归的过程
   quicksort(i + 1, right); //继续处理右边的 ，这里是一个递归的过程
}
int main()
{
   // Node_link_list c(3);
   int len = 10;
   link_list list1 = link_list(5);
   //遍历
   list_ergodic(list1);
   cout << "----------insert element------" << endl;
   insert_element_ll(1, list1, 5);
   list_ergodic(list1);
   cout << "-------delement_ll---------" << endl;
   dele_element_ll(119, list1);
   list_ergodic(list1);
   cout << "----------------end insert------------------" << endl;
   insert_end_ll(9, list1);
   list_ergodic(list1);
   // return 1;
   // quicksort

   //读入数据
   scanf("%d", &n);
   for (i = 1; i <= n; i++)
      scanf("%d", &a[i]);
   quicksort(1, n); //快速排序调用
   //输出排序后的结果
   for (i = 1; i <= n; i++)
      printf("%d ", a[i]);
   getchar();
   getchar();
   return 0;
}