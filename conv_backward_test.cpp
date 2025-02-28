#include <iostream>
#include <cassert>
#include "./matrix/matrix_def.h"
#include "./matrix/matrix_pro.h"
#include "./root/include/edgelayer.h"

// 辅助函数：检查两个浮点数是否近似相等
bool is_close(float a, float b, float rtol = 1e-5, float atol = 1e-8) {
    return fabs(a - b) <= (atol + rtol * fabs(b));
}

int main() {
    // 创建测试输入 (batch=1, channels=3, height=4, width=4)
    Matrix4d input = CreateMatrix4d(1, 3, 4, 4);
    
    // 用固定的测试值填充输入
    float test_input[3][4][4] = {
        {{1,2,3,4}, {2,3,4,5}, {3,4,5,6}, {4,5,6,7}},  // channel 1
        {{2,4,6,8}, {4,6,8,10}, {6,8,10,12}, {8,10,12,14}},  // channel 2
        {{3,6,9,12}, {6,9,12,15}, {9,12,15,18}, {12,15,18,21}}  // channel 3
    };
    
    for(int c = 0; c < 3; c++) {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                input.matrix4d[0].matrix3d[c].matrix[i][j] = test_input[c][i][j];
            }
        }
    }
    
    // 创建卷积层并设置固定的卷积核
    edge_layer* conv = new conv2d(input, 3, 2, 1, 3, 0, 1);
    conv2d* conv_ptr = dynamic_cast<conv2d*>(conv);
    
    // 设置固定的卷积核值
    float test_kernel[3][2][3][3] = {
        {{{1,0,1}, {0,1,0}, {1,0,1}},  // in_channel 0, out_channel 0
         {{0,1,0}, {1,0,1}, {0,1,0}}}, // in_channel 0, out_channel 1
        {{{1,1,0}, {1,0,1}, {0,1,1}},  // in_channel 1, out_channel 0
         {{0,1,1}, {1,0,1}, {1,1,0}}}, // in_channel 1, out_channel 1
        {{{0,0,1}, {0,1,0}, {1,0,0}},  // in_channel 2, out_channel 0
         {{1,0,0}, {0,1,0}, {0,0,1}}}  // in_channel 2, out_channel 1
    };
    
    for(int in_c = 0; in_c < 3; in_c++) {
        for(int out_c = 0; out_c < 2; out_c++) {
            conv_ptr->set_kernel(&test_kernel[in_c][out_c][0][0], in_c, out_c);
        }
    }
    
    // 前向传播
    Matrix4d output = conv->forward(input);
    std::cout << "Forward output shape:" << std::endl;
    print_shape(output);
    std::cout << "Forward output values:" << std::endl;
    cout_mat4d(output);
    
    // 验证前向传播的输出尺寸
    assert(output.batch == 1);
    assert(output.dep == 2);
    // 计算期望的输出尺寸
    int expected_size = ((4 + 2*1 - 3) / 1) + 1;  // ((4 + 2 - 3) / 1) + 1 = 4
    assert(output.wid == expected_size);  
    assert(output.high == expected_size);
    
    // 创建固定的梯度输出
    int output_size = ((4 + 2*1 - 3) / 1) + 1;  // 计算正确的输出尺寸
    Matrix4d grad_output = CreateMatrix4d(1, 2, output_size, output_size);
    float test_grad[2][4][4] = {
        {{1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}},  // out_channel 0
        {{0.5,0.5,0.5,0.5}, {0.5,0.5,0.5,0.5}, {0.5,0.5,0.5,0.5}, {0.5,0.5,0.5,0.5}}  // out_channel 1
    };
    
    for(int c = 0; c < 2; c++) {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                grad_output.matrix4d[0].matrix3d[c].matrix[i][j] = test_grad[c][i][j];
            }
        }
    }
    
    // 反向传播
    Matrix4d grad_input = conv->backward(grad_output);
    std::cout << "Backward gradients shape:" << std::endl;
    print_shape(grad_input);
    std::cout << "Backward gradients values:" << std::endl;
    cout_mat4d(grad_input);
    
    // 验证反向传播的梯度尺寸
    assert(grad_input.batch == 1);
    assert(grad_input.dep == 3);
    assert(grad_input.wid == 4);
    assert(grad_input.high == 4);
    
    // 验证一些特定位置的梯度值
    // 手动计算的预期梯度值
    float expected_grads[3][4][4] = {
        // channel 0
        {
            {2.0f, 1.5f, 1.5f, 1.0f},
            {1.5f, 2.0f, 2.0f, 1.5f},
            {1.5f, 2.0f, 2.0f, 1.5f},
            {1.0f, 1.5f, 1.5f, 1.0f}
        },
        // channel 1
        {
            {2.5f, 2.0f, 2.0f, 1.5f},
            {2.0f, 3.0f, 3.0f, 2.0f},
            {2.0f, 3.0f, 3.0f, 2.0f},
            {1.5f, 2.0f, 2.0f, 1.5f}
        },
        // channel 2
        {
            {1.5f, 1.0f, 1.0f, 0.5f},
            {1.0f, 1.5f, 1.5f, 1.0f},
            {1.0f, 1.5f, 1.5f, 1.0f},
            {0.5f, 1.0f, 1.0f, 0.5f}
        }
    };
    
    for(int c = 0; c < 3; c++) {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                float actual = grad_input.matrix4d[0].matrix3d[c].matrix[i][j];
                float expected = expected_grads[c][i][j];
                if (!is_close(actual, expected)) {
                    std::cout << "Gradient mismatch at position [" << c << "][" << i << "][" << j << "]" << std::endl;
                    std::cout << "Expected: " << expected << ", Got: " << actual << std::endl;
                }
            }
        }
    }
    
    std::cout << "All tests passed!" << std::endl;
    
    delete conv;
    return 0;
} 