#include <math.h>
#include <time.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include "./matrix/matrix_def.h"
#include "./matrix/matrix_pro.h"
#include "./logistic/logistic_def.h"
#include "./autodiff/node.h"
#include "./grad_edge/matrix_grad.h"
#include "./root/include/edgelayer.h"
#include "./welcome/score_wel.cpp"
#include <iomanip>
#include <iostream>
#include <cassert>
using namespace std;

// TODO: 上面的函数定义

void test_batch_conv_test(int batch_size,
                          int depth, 
                          int height, 
                          int width, 
                          int output_channels, 
                          int stride, 
                          int kernel_size, 
                          int mode) 
    {
    // 打印参数对应关系
    cout << "Parameters:" << endl;
    cout << "batch_size = " << batch_size << endl;
    cout << "depth = " << depth << endl;
    cout << "height = " << height << endl;
    cout << "width = " << width << endl;
    cout << "output_channels = " << output_channels << endl;
    cout << "stride = " << stride << endl;
    cout << "kernel_size = " << kernel_size << endl;
    cout << "mode = " << mode << endl;

    Matrix4d input4d = CreateMatrix4d(batch_size, depth, height, width);
    Matrix4d output4d = batch_conv_test(input4d, depth, output_channels, stride, kernel_size, mode,true);
    // 打印卷积维度计算过程
    int padding_wid = stride - (input4d.wid - kernel_size) % stride;
    if (padding_wid == stride) {
        padding_wid = 0;
    }
    int padding_high = stride - (input4d.high - kernel_size) % stride;
    if (padding_high == stride) {
        padding_high = 0;
    }
    int output_width = (width - kernel_size + 2 * padding_wid) / stride + 1;
    int output_height = (height - kernel_size + 2 * padding_high) / stride + 1;
    cout << "Output dimension calculation:" << endl;
    cout << "output_width = (" << width << " - " << kernel_size << " + 2 * " << padding_wid << ") / " << stride << " + 1 = " << output_width << endl;
    cout << "output_height = (" << height << " - " << kernel_size << " + 2 * " << padding_high << ") / " << stride << " + 1 = " << output_height << endl;
    getshape4d(output4d);
    assert(output4d.dep == output_channels);
    assert(output4d.wid == output_width);
    assert(output4d.high == output_height);
    }
int main() {
    test_batch_conv_test(2, 3, 4, 4, 2, 1, 2, 0);
    cout << "All tests passed!\n";
    return 0;
}
