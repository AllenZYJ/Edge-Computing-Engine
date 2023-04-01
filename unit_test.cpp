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
    Matrix4d output4d = batch_conv_test(input4d, depth, output_channels, stride, kernel_size, mode,false);
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
ofstream outfile("result.txt");  // 打开输出文件流
outfile << "bs" << "\t" << "ic" << "\t" << "height" << "\t" << "width" << "\t" 
                                << "oc" << "\t" << "stride" << "\t" << "ksize" << "\t" << "mode" << endl;
    test_batch_conv_test(1, 3, 32, 32, 1, 2, 3, 0);
    for (int width = 32; width <= 640; ++width) {
    for (int height = 32; height <= 640; ++height) {
        for (int output_channels = 1; output_channels <= 256; ++output_channels) {
            for (int kernel_size = 1; kernel_size <= 7; ++kernel_size) {
                for (int stride = 1; stride <= kernel_size; ++stride) {
                    for (int batch_size = 1; batch_size <= 4; ++batch_size) {
                        cout<<"---------------------------"<<endl;
                        outfile << batch_size << "\t" << 3 << "\t" << height << "\t" << width << "\t" 
                                << output_channels << "\t" << stride << "\t" << kernel_size << "\t" << 0 << endl;
                        test_batch_conv_test(batch_size, 3, height, width, output_channels, stride, kernel_size, 0);
                    }
                }
            }
        }
    }
}
    cout << "All tests passed!\n";
    return 0;
}
