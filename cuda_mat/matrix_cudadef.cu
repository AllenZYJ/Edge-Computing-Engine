#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cuda_runtime.h>
#include <cassert>
#include <cstring>
#include <cmath>

#define CHECK_CUDA_ERROR(val) check((val), #val, __FILE__, __LINE__)
template <typename T>
void check(T err, const char* const func, const char* const file, const int line) {
    if (err != cudaSuccess) {
        std::cerr << "CUDA Runtime Error at: " << file << ":" << line << std::endl;
        std::cerr << cudaGetErrorString(err) << " " << func << std::endl;
        exit(1);
    }
}

struct Matrix_CU {
    int row;
    int col;
    float* data;

    Matrix_CU(int r, int c) : row(r), col(c) {
        data = new float[r * c]();
    }
    // 删除拷贝构造函数和拷贝赋值运算符
    Matrix_CU(const Matrix_CU& other) : row(other.row), col(other.col) {
        data = new float[row * col];
        std::memcpy(data, other.data, row * col * sizeof(float));
    }

    // 拷贝赋值运算符
    Matrix_CU& operator=(const Matrix_CU& other) {
        if (this != &other) {
            delete[] data;
            row = other.row;
            col = other.col;
            data = new float[row * col];
            std::memcpy(data, other.data, row * col * sizeof(float));
        }
        return *this;
    }
    ~Matrix_CU() {
        delete[] data;
    }

    void randomInit() {
        for (int i = 0; i < row * col; ++i) {
            data[i] = static_cast<float>(rand()) / RAND_MAX;
        }
    }

    void printFirstElement() const {
        std::cout << "First element: " << data[0] << std::endl;
    }
    void printSubMatrix(int numRows, int numCols) const {
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            std::cout << data[i * col + j] << " ";
        }
        std::cout << std::endl;
        }
    }
    
};
float getMaxValue(const Matrix_CU& matrix) {
    float maxVal = matrix.data[0];
    for (int i = 1; i < matrix.row * matrix.col; ++i) {
            if (matrix.data[i] > maxVal) {
                maxVal = matrix.data[i];
            }
        }
        return maxVal;
    }
float getMinValue(const Matrix_CU& matrix) {
    float minVal = matrix.data[0];
    for (int i = 1; i < matrix.row * matrix.col; ++i) {
        if (matrix.data[i] < minVal) {
            minVal = matrix.data[i];
        }
    }
    return minVal;
}
float getAverageValue(const Matrix_CU& matrix) {
    float sum = 0.0f;
    for (int i = 0; i < matrix.row * matrix.col; ++i) {
        sum += matrix.data[i];
    }
    return sum / (matrix.row * matrix.col);
}
float getSum(const Matrix_CU& matrix) {
    float sum = 0.0f;
    for (int i = 0; i < matrix.row * matrix.col; ++i) {
        sum += matrix.data[i];
    }
    return sum;
}

float getStandardDeviation(const Matrix_CU& matrix) {
    float mean = getAverageValue(matrix);
    float sumSquaredDifferences = 0.0f;
    
    for (int i = 0; i < matrix.row * matrix.col; ++i) {
        float diff = matrix.data[i] - mean;
        sumSquaredDifferences += diff * diff;
    }

    return sqrt(sumSquaredDifferences / (matrix.row * matrix.col));
}

__global__ void matrixAddKernel(const float* A, const float* B, float* C, int rows, int cols) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int idy = blockIdx.y * blockDim.y + threadIdx.y;
    
    if (idx < rows && idy < cols) {
        int linear_idx = idx * cols + idy;
        C[linear_idx] = A[linear_idx] + B[linear_idx];
    }
}

// 修改为直接操作输出矩阵，避免拷贝
void matrixAddCUDA(const Matrix_CU& A, const Matrix_CU& B, Matrix_CU& C) {
    assert(A.row == B.row && A.col == B.col);
    assert(A.row == C.row && A.col == C.col);
    
    const int rows = A.row;
    const int cols = A.col;
    size_t size = rows * cols * sizeof(float);
    
    float *d_A, *d_B, *d_C;
    
    // 1. 分配设备内存
    CHECK_CUDA_ERROR(cudaMalloc(&d_A, size));
    CHECK_CUDA_ERROR(cudaMalloc(&d_B, size));
    CHECK_CUDA_ERROR(cudaMalloc(&d_C, size));

    // 2. 拷贝数据到设备
    CHECK_CUDA_ERROR(cudaMemcpy(d_A, A.data, size, cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaMemcpy(d_B, B.data, size, cudaMemcpyHostToDevice));

    // 3. 配置内核参数
    dim3 blockSize(16, 16); // 256 threads per block
    dim3 gridSize((cols + blockSize.x - 1) / blockSize.x,
                 (rows + blockSize.y - 1) / blockSize.y);
    auto start = std::chrono::high_resolution_clock::now();
    // 4. 启动核函数
    matrixAddKernel<<<gridSize, blockSize>>>(d_A, d_B, d_C, rows, cols);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "GPU执行时间: " << elapsed.count() * 1000 << " ms\n";
    CHECK_CUDA_ERROR(cudaGetLastError()); // 检查内核启动错误
    
    // 5. 拷贝结果回主机
    CHECK_CUDA_ERROR(cudaMemcpy(C.data, d_C, size, cudaMemcpyDeviceToHost));

    // 6. 释放设备内存
    CHECK_CUDA_ERROR(cudaFree(d_A));
    CHECK_CUDA_ERROR(cudaFree(d_B));
    CHECK_CUDA_ERROR(cudaFree(d_C));
}

Matrix_CU matrixAddCPU(const Matrix_CU& A, const Matrix_CU& B) {
    assert(A.row == B.row && A.col == B.col);
    
    Matrix_CU C(A.row, A.col);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < A.row * A.col; ++i) {
        C.data[i] = A.data[i] + B.data[i];
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "CPU执行时间: " << elapsed.count() * 1000 << " ms\n";
    
    return C;
}

void testMatrix_CUAddition() {
    const int rows = 10240;
    const int cols = 10240;
    std::cout << "测试矩阵大小: " << rows << "x" << cols << "\n";
    
    Matrix_CU A(rows, cols);
    Matrix_CU B(rows, cols);
    Matrix_CU C_gpu(rows, cols);  // GPU结果矩阵
    
    A.randomInit();
    B.randomInit();
    
    std::cout << "矩阵A: ";
    A.printFirstElement();
    std::cout << "矩阵B: ";
    B.printFirstElement();

    std::cout << "\nCPU版本:\n";
    Matrix_CU C_cpu = matrixAddCPU(A, B);
    std::cout << "CPU结果: ";
    C_cpu.printFirstElement();

    std::cout << "\nGPU版本:\n";
    matrixAddCUDA(A, B, C_gpu);
    std::cout << "GPU结果: ";
    C_gpu.printFirstElement();
    std::cout<< "标准差:" << getStandardDeviation(C_gpu);
    std::cout << "最大值: " << getMaxValue(C_gpu) << std::endl;
    std::cout << "最小值: " << getMinValue(C_gpu) << std::endl;
    std::cout << "平均值: " << getAverageValue(C_gpu) << std::endl;
    std::cout << "和: " << getSum(C_gpu) << std::endl;
    std::cout << "标准差: " << getStandardDeviation(C_gpu) << std::endl;
    // 验证结果
    bool correct = true;
    for (int i = 0; i < 10 && correct; ++i) {
        if (fabs(C_cpu.data[i] - C_gpu.data[i]) > 1e-5) {
            std::cout << "结果不匹配在索引 " << i 
                      << ": CPU=" << C_cpu.data[i] 
                      << ", GPU=" << C_gpu.data[i] << "\n";
            correct = false;
        }
    }
    
    if (correct) {
        std::cout << "前10个元素验证成功!\n";
    }
}

int main() {
    // 初始化CUDA设备
    cudaDeviceProp prop;
    CHECK_CUDA_ERROR(cudaGetDeviceProperties(&prop, 0));
    std::cout << "使用GPU: " << prop.name << "\n";
    
    testMatrix_CUAddition();
    return 0;
}