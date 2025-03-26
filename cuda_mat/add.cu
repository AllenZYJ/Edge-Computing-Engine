#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cuda_runtime.h>

// CUDA 核函数：向量加法
__global__ void vectorAdd(const float *A, const float *B, float *C, int numElements)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < numElements)
    {
        C[i] = A[i] + B[i];
    }
}

int main()
{
    // 设置向量大小 (1百万个元素)
    int numElements = 1 << 20;
    size_t size = numElements * sizeof(float);
    std::cout << "向量加法测试，每个向量包含 " << numElements << " 个元素\n";

    // 分配主机内存
    float *h_A = new float[numElements];
    float *h_B = new float[numElements];
    float *h_C = new float[numElements];
    float *d_A = nullptr;
    float *d_B = nullptr;
    float *d_C = nullptr;

    // 初始化输入向量
    for (int i = 0; i < numElements; ++i)
    {
        h_A[i] = rand() / (float)RAND_MAX;
        h_B[i] = rand() / (float)RAND_MAX;
    }

    // 分配设备内存
    cudaMalloc((void **)&d_A, size);
    cudaMalloc((void **)&d_B, size);
    cudaMalloc((void **)&d_C, size);

    // 拷贝数据到设备
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // 启动 CUDA 核函数
    int threadsPerBlock = 256;
    int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;
    std::cout << "CUDA 核函数配置: " << blocksPerGrid << " 个块, 每个块 " << threadsPerBlock << " 个线程\n";

    // 测试 GPU 性能
    auto start_gpu = std::chrono::high_resolution_clock::now();
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, numElements);
    cudaDeviceSynchronize(); // 等待 GPU 完成
    auto end_gpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> gpu_time = end_gpu - start_gpu;
    std::cout << "GPU 执行时间: " << gpu_time.count() * 1000 << " 毫秒\n";

    // 拷贝结果回主机
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    // 测试 CPU 性能
    auto start_cpu = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numElements; ++i)
    {
        h_C[i] = h_A[i] + h_B[i];
    }
    auto end_cpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> cpu_time = end_cpu - start_cpu;
    std::cout << "CPU 执行时间: " << cpu_time.count() * 1000 << " 毫秒\n";

    // 验证结果
    for (int i = 0; i < numElements; i++)
    {
        if (fabs(h_A[i] + h_B[i] - h_C[i]) > 1e-5)
        {
            std::cerr << "结果验证失败!\n";
            break;
        }
    }

    // 释放内存
    delete[] h_A;
    delete[] h_B;
    delete[] h_C;
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    std::cout << "测试完成\n";

    return 0;
}