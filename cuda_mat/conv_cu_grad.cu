#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cuda_runtime.h>
#include <cassert>
#include <cstring>
#include <cmath>
#include <functional>
#include <vector>
#include <iomanip>
#include "mat_grad.cu"

// 已有的CPU卷积实现
void conv2dCPU(const float* input, const float* kernel, float* output,
              int batch_size, int in_channels, int out_channels,
              int in_height, int in_width, int kernel_height, int kernel_width,
              int out_height, int out_width, int stride_height, int stride_width,
              int pad_height, int pad_width) {
    
    for (int n = 0; n < batch_size; n++) {
        for (int c = 0; c < out_channels; c++) {
            for (int h = 0; h < out_height; h++) {
                for (int w = 0; w < out_width; w++) {
                    float sum = 0.0f;
                    
                    for (int ic = 0; ic < in_channels; ic++) {
                        for (int kh = 0; kh < kernel_height; kh++) {
                            for (int kw = 0; kw < kernel_width; kw++) {
                                int ih = h * stride_height + kh - pad_height;
                                int iw = w * stride_width + kw - pad_width;
                                
                                if (ih >= 0 && ih < in_height && iw >= 0 && iw < in_width) {
                                    int input_idx = ((n * in_channels + ic) * in_height + ih) * in_width + iw;
                                    int kernel_idx = ((c * in_channels + ic) * kernel_height + kh) * kernel_width + kw;
                                    
                                    sum += input[input_idx] * kernel[kernel_idx];
                                }
                            }
                        }
                    }
                    
                    int output_idx = ((n * out_channels + c) * out_height + h) * out_width + w;
                    output[output_idx] = sum;
                }
            }
        }
    }
}

// 新增的速度比较函数
void compareConvolutionSpeed() {
    std::cout << "\n====== CPU vs GPU Convolution Speed Comparison ======\n";
    
    // 定义不同的测试配置
    struct TestConfig {
        int batch_size;
        int in_channels;
        int in_height;
        int in_width;
        int out_channels;
        int kernel_size;
        int stride;
        int padding;
    };
    
    std::vector<TestConfig> configs = {
        {1, 3, 32, 32, 16, 3, 1, 1},      // 小尺寸
        {4, 3, 64, 64, 32, 3, 1, 1},      // 中等尺寸
        {16, 3, 128, 128, 64, 3, 1, 1},   // 大尺寸
        {32, 3, 224, 224, 64, 3, 1, 1},   // 类似VGG的输入
        {32, 64, 56, 56, 128, 3, 1, 1},   // 类似中层特征图
        {16, 128, 28, 28, 256, 3, 1, 1},  // 类似深层特征图
    };
    
    // 表头
    std::cout << std::left << std::setw(20) << "配置" 
              << std::setw(15) << "输入尺寸" 
              << std::setw(15) << "内核尺寸" 
              << std::setw(15) << "CPU时间(ms)" 
              << std::setw(15) << "GPU时间(ms)" 
              << std::setw(15) << "加速比(x)" 
              << std::endl;
    std::cout << std::string(95, '-') << std::endl;
    
    // 对每个配置进行测试
    for (size_t i = 0; i < configs.size(); i++) {
        const auto& cfg = configs[i];
        
        // 计算输出尺寸
        int out_height = ((cfg.in_height + 2 * cfg.padding - cfg.kernel_size) / cfg.stride) + 1;
        int out_width = ((cfg.in_width + 2 * cfg.padding - cfg.kernel_size) / cfg.stride) + 1;
        
        // 创建计算图和节点(GPU)
        ComputeGraph graph;
        ComputeNode* input = graph.addInput(cfg.batch_size * cfg.in_channels, cfg.in_height * cfg.in_width);
        input->batchSize = cfg.batch_size;
        ComputeNode* kernel = graph.addInput(cfg.out_channels, cfg.in_channels * cfg.kernel_size * cfg.kernel_size);
        
        // 随机初始化值
        for (int i = 0; i < input->value.row * input->value.col; i++) {
            input->value.data[i] = static_cast<float>(rand()) / RAND_MAX;
        }
        
        for (int i = 0; i < kernel->value.row * kernel->value.col; i++) {
            kernel->value.data[i] = static_cast<float>(rand()) / RAND_MAX * 0.1f;
        }
        
        // 创建卷积节点
        ComputeNode* conv = graph.addConv2D(input, kernel, cfg.in_height, cfg.in_width, cfg.in_channels, 
                                          cfg.stride, cfg.stride, cfg.padding, cfg.padding);
        
        // 为CPU实现准备数据
        std::vector<float> input_cpu(cfg.batch_size * cfg.in_channels * cfg.in_height * cfg.in_width);
        std::vector<float> kernel_cpu(cfg.out_channels * cfg.in_channels * cfg.kernel_size * cfg.kernel_size);
        std::vector<float> output_cpu(cfg.batch_size * cfg.out_channels * out_height * out_width, 0.0f);
        
        // 重塑输入数据为CPU格式
        for (int n = 0; n < cfg.batch_size; n++) {
            for (int c = 0; c < cfg.in_channels; c++) {
                for (int h = 0; h < cfg.in_height; h++) {
                    for (int w = 0; w < cfg.in_width; w++) {
                        int flat_idx = (n * cfg.in_channels + c) * (cfg.in_height * cfg.in_width) + (h * cfg.in_width + w);
                        int tensor_idx = ((n * cfg.in_channels + c) * cfg.in_height + h) * cfg.in_width + w;
                        input_cpu[tensor_idx] = input->value.data[flat_idx];
                    }
                }
            }
        }
        
        // 重塑内核数据为CPU格式
        for (int oc = 0; oc < cfg.out_channels; oc++) {
            for (int ic = 0; ic < cfg.in_channels; ic++) {
                for (int kh = 0; kh < cfg.kernel_size; kh++) {
                    for (int kw = 0; kw < cfg.kernel_size; kw++) {
                        int flat_idx = oc * (cfg.in_channels * cfg.kernel_size * cfg.kernel_size) + 
                                     (ic * cfg.kernel_size * cfg.kernel_size + kh * cfg.kernel_size + kw);
                        int tensor_idx = ((oc * cfg.in_channels + ic) * cfg.kernel_size + kh) * cfg.kernel_size + kw;
                        kernel_cpu[tensor_idx] = kernel->value.data[flat_idx];
                    }
                }
            }
        }
        
        // 计时器
        std::chrono::duration<double, std::milli> gpu_duration;
        std::chrono::duration<double, std::milli> cpu_duration;
        
        // GPU卷积计时（多次运行取平均值）
        const int num_runs = 10;
        auto gpu_start = std::chrono::high_resolution_clock::now();
        for (int run = 0; run < num_runs; run++) {
            graph.forward();
            // 为避免结果被缓存，修改输入小量值
            input->value.data[0] += 1e-5f;
        }
        auto gpu_end = std::chrono::high_resolution_clock::now();
        gpu_duration = (gpu_end - gpu_start) / num_runs;
        
        // CPU卷积计时（对于大尺寸配置减少运行次数）
        int cpu_runs = (i < 3) ? num_runs : 3;  // 大尺寸配置少运行几次
        auto cpu_start = std::chrono::high_resolution_clock::now();
        for (int run = 0; run < cpu_runs; run++) {
            conv2dCPU(input_cpu.data(), kernel_cpu.data(), output_cpu.data(),
                     cfg.batch_size, cfg.in_channels, cfg.out_channels,
                     cfg.in_height, cfg.in_width, cfg.kernel_size, cfg.kernel_size,
                     out_height, out_width, cfg.stride, cfg.stride,
                     cfg.padding, cfg.padding);
        }
        auto cpu_end = std::chrono::high_resolution_clock::now();
        cpu_duration = (cpu_end - cpu_start) / cpu_runs;
        
        // 计算加速比
        double speedup = cpu_duration.count() / gpu_duration.count();
        
        // 打印结果
        std::cout << std::left << std::setw(20) << "配置 " + std::to_string(i+1)
                  << std::setw(15) << cfg.batch_size * cfg.in_channels * cfg.in_height * cfg.in_width
                  << std::setw(15) << cfg.out_channels * cfg.in_channels * cfg.kernel_size * cfg.kernel_size
                  << std::setw(15) << std::fixed << std::setprecision(2) << cpu_duration.count()
                  << std::setw(15) << std::fixed << std::setprecision(2) << gpu_duration.count()
                  << std::setw(15) << std::fixed << std::setprecision(2) << speedup
                  << std::endl;
    }
    
    std::cout << "\nCPU和GPU卷积速度比较完成\n";
}

// 更新main函数以包含速度比较
int main() {
    // 设置随机数种子
    srand(42);
    
    // 运行速度比较测试
// 配置              输入尺寸   内核尺寸   CPU时间(ms)  GPU时间(ms)  加速比(x)
// -----------------------------------------------------------------------------------------------
// 配置 1            3072           432            2.01           5.70           0.35
// 配置 2            49152          864            65.87          6.36           10.35
// 配置 3            786432         1728           2126.64        212.35         10.01
// 配置 4            4816896        1728           12969.35       1319.69        9.83
// 配置 5            6422528        73728          33261.28       387.02         85.94
// 配置 6            1605632        294912         16521.94       140.25         117.80
    compareConvolutionSpeed();
    
    return 0;
}