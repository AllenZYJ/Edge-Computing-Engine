#include "matrix_cudadef.cu"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cuda_runtime.h>
#include <cassert>
#include <cstring>
#include <cmath>
#include <functional>
// 自动微分节点类型枚举
enum OpType {
    OP_INPUT,    // 输入节点
    OP_ADD,      // 加法
    OP_SUB,      // 减法
    OP_MUL,      // 元素级乘法
    OP_DIV,      // 元素级除法
    OP_MATMUL,   // 矩阵乘法
    OP_RELU,     // ReLU激活函数
    OP_SIGMOID,  // Sigmoid激活函数
    OP_TANH,     // Tanh激活函数
    OP_SOFTMAX,  // Softmax函数
    OP_CONV2D    // 2D卷积
};
struct ConvParams {
    int in_channels;
    int out_channels;
    int kernel_height;
    int kernel_width;
    int stride_height;
    int stride_width;
    int pad_height;
    int pad_width;
    int in_height;
    int in_width;
    int out_height;
    int out_width;
    int batch_size;
};
// 计算图节点
struct ComputeNode {
    OpType op;                     // 操作类型
    Matrix_CU value;               // 前向传播值
    Matrix_CU grad;                // 梯度值
    std::vector<ComputeNode*> inputs;  // 输入节点
    int batchSize;                 // 批处理大小
    ConvParams conv_params; 
    ComputeNode(int rows, int cols, int batch = 1) : 
        op(OP_INPUT), 
        value(rows * batch, cols), 
        grad(rows * batch, cols), 
        batchSize(batch) {}
    
    // 防止内存泄漏
    ~ComputeNode() {
        // 析构函数会自动处理value和grad
    }
    
    // 重置梯度
    void zeroGrad() {
        std::memset(grad.data, 0, grad.row * grad.col * sizeof(float));
    }
};

// CUDA核函数：ReLU前向传播
__global__ void reluForwardKernel(const float* input, float* output, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        output[idx] = fmaxf(0.0f, input[idx]);
    }
}

// CUDA核函数：ReLU反向传播
__global__ void reluBackwardKernel(const float* outGrad, const float* input, float* inGrad, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        inGrad[idx] = (input[idx] > 0.0f) ? outGrad[idx] : 0.0f;
    }
}

// CUDA核函数：Sigmoid前向传播
__global__ void sigmoidForwardKernel(const float* input, float* output, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        output[idx] = 1.0f / (1.0f + expf(-input[idx]));
    }
}

// CUDA核函数：Sigmoid反向传播
__global__ void sigmoidBackwardKernel(const float* outGrad, const float* output, float* inGrad, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        // sigmoid的导数是 sigmoid(x) * (1 - sigmoid(x))
        inGrad[idx] = outGrad[idx] * output[idx] * (1.0f - output[idx]);
    }
}

// CUDA核函数：矩阵乘法前向传播 (C = A * B)
__global__ void matmulForwardKernel(const float* A, const float* B, float* C, 
                                   int A_rows, int A_cols, int B_cols) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (row < A_rows && col < B_cols) {
        float sum = 0.0f;
        for (int k = 0; k < A_cols; ++k) {
            sum += A[row * A_cols + k] * B[k * B_cols + col];
        }
        C[row * B_cols + col] = sum;
    }
}

// CUDA核函数：矩阵乘法反向传播 - A的梯度
__global__ void matmulBackwardAKernel(const float* outGrad, const float* B, float* inGradA,
                                     int outGrad_rows, int B_rows, int B_cols) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (row < outGrad_rows && col < B_rows) {
        float sum = 0.0f;
        for (int k = 0; k < B_cols; ++k) {
            sum += outGrad[row * B_cols + k] * B[col * B_cols + k];
        }
        inGradA[row * B_rows + col] = sum;
    }
}

// CUDA核函数：矩阵乘法反向传播 - B的梯度
__global__ void matmulBackwardBKernel(const float* A, const float* outGrad, float* inGradB,
                                     int A_rows, int A_cols, int outGrad_cols) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (row < A_cols && col < outGrad_cols) {
        float sum = 0.0f;
        for (int k = 0; k < A_rows; ++k) {
            sum += A[k * A_cols + row] * outGrad[k * outGrad_cols + col];
        }
        inGradB[row * outGrad_cols + col] = sum;
    }
}
// CUDA kernel for convolution forward pass
__global__ void conv2dForwardKernel(
    const float* input, const float* kernel, float* output,
    int batch_size, int in_channels, int out_channels,
    int in_height, int in_width, int kernel_height, int kernel_width,
    int out_height, int out_width, int stride_height, int stride_width,
    int pad_height, int pad_width) {
    
    int n = blockIdx.x;  // batch index
    int c = blockIdx.y;  // output channel index
    
    int h = (blockIdx.z / out_width);
    int w = (blockIdx.z % out_width);
    
    if (n >= batch_size || c >= out_channels || h >= out_height || w >= out_width)
        return;
    
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
// CUDA kernel for computing gradients with respect to input
__global__ void conv2dBackwardInputKernel(
    const float* outGrad, const float* kernel, float* inGrad,
    int batch_size, int in_channels, int out_channels,
    int in_height, int in_width, int kernel_height, int kernel_width,
    int out_height, int out_width, int stride_height, int stride_width,
    int pad_height, int pad_width) {
    
    int n = blockIdx.x;  // batch index
    int ic = blockIdx.y; // input channel index
    int idx = blockIdx.z;
    int h = idx / in_width;
    int w = idx % in_width;
    
    if (n >= batch_size || ic >= in_channels || h >= in_height || w >= in_width)
        return;
    
    float sum = 0.0f;
    
    for (int oc = 0; oc < out_channels; oc++) {
        for (int kh = 0; kh < kernel_height; kh++) {
            for (int kw = 0; kw < kernel_width; kw++) {
                int oh = (h + pad_height - kh) / stride_height;
                int ow = (w + pad_width - kw) / stride_width;
                
                // Check if the computed output location is valid and if it aligns with stride
                if (oh >= 0 && oh < out_height && ow >= 0 && ow < out_width &&
                    (h + pad_height - kh) % stride_height == 0 &&
                    (w + pad_width - kw) % stride_width == 0) {
                    
                    int out_grad_idx = ((n * out_channels + oc) * out_height + oh) * out_width + ow;
                    int kernel_idx = ((oc * in_channels + ic) * kernel_height + kh) * kernel_width + kw;
                    
                    sum += outGrad[out_grad_idx] * kernel[kernel_idx];
                }
            }
        }
    }
    
    int in_grad_idx = ((n * in_channels + ic) * in_height + h) * in_width + w;
    inGrad[in_grad_idx] += sum;  // Use += for gradient accumulation
}

// CUDA kernel for computing gradients with respect to kernel
__global__ void conv2dBackwardKernelKernel(
    const float* input, const float* outGrad, float* kernelGrad,
    int batch_size, int in_channels, int out_channels,
    int in_height, int in_width, int kernel_height, int kernel_width,
    int out_height, int out_width, int stride_height, int stride_width,
    int pad_height, int pad_width) {
    
    int oc = blockIdx.x;  // output channel index
    int ic = blockIdx.y;  // input channel index
    int idx = blockIdx.z;
    int kh = idx / kernel_width;
    int kw = idx % kernel_width;
    
    if (oc >= out_channels || ic >= in_channels || kh >= kernel_height || kw >= kernel_width)
        return;
    
    float sum = 0.0f;
    
    for (int n = 0; n < batch_size; n++) {
        for (int oh = 0; oh < out_height; oh++) {
            for (int ow = 0; ow < out_width; ow++) {
                int ih = oh * stride_height + kh - pad_height;
                int iw = ow * stride_width + kw - pad_width;
                
                if (ih >= 0 && ih < in_height && iw >= 0 && iw < in_width) {
                    int input_idx = ((n * in_channels + ic) * in_height + ih) * in_width + iw;
                    int out_grad_idx = ((n * out_channels + oc) * out_height + oh) * out_width + ow;
                    
                    sum += input[input_idx] * outGrad[out_grad_idx];
                }
            }
        }
    }
    
    int kernel_grad_idx = ((oc * in_channels + ic) * kernel_height + kh) * kernel_width + kw;
    kernelGrad[kernel_grad_idx] = sum;
}

// 为加法和减法操作修改反向传播实现
void addBackwardCUDA(const Matrix_CU& outGrad, Matrix_CU& inGradA, Matrix_CU& inGradB) {
    assert(outGrad.row == inGradA.row && outGrad.col == inGradA.col);
    assert(outGrad.row == inGradB.row && outGrad.col == inGradB.col);
    
    // 加法的梯度直接复制到两个输入（主机内存到主机内存）
    const int size = outGrad.row * outGrad.col * sizeof(float);
    std::memcpy(inGradA.data, outGrad.data, size);
    std::memcpy(inGradB.data, outGrad.data, size);
}
// 添加一个全局的CUDA核函数，替代lambda函数
__global__ void negGradKernel(const float* outGrad, float* inGrad, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        inGrad[idx] = -outGrad[idx];
    }
}
void subBackwardCUDA(const Matrix_CU& outGrad, Matrix_CU& inGradA, Matrix_CU& inGradB) {
    assert(outGrad.row == inGradA.row && outGrad.col == inGradA.col);
    assert(outGrad.row == inGradB.row && outGrad.col == inGradB.col);
    
    const int rows = outGrad.row;
    const int cols = outGrad.col;
    const int totalElements = rows * cols;
    const size_t size = totalElements * sizeof(float);
    
    // 对于A: 直接复制梯度（主机内存到主机内存）
    std::memcpy(inGradA.data, outGrad.data, size);
    
    // 对于B: 计算负梯度
    float *d_outGrad, *d_inGradB;
    CHECK_CUDA_ERROR(cudaMalloc(&d_outGrad, size));
    CHECK_CUDA_ERROR(cudaMalloc(&d_inGradB, size));
    
    CHECK_CUDA_ERROR(cudaMemcpy(d_outGrad, outGrad.data, size, cudaMemcpyHostToDevice));
    
    int blockSize = 256;
    int numBlocks = (totalElements + blockSize - 1) / blockSize;
    
    negGradKernel<<<numBlocks, blockSize>>>(d_outGrad, d_inGradB, totalElements);
    
    CHECK_CUDA_ERROR(cudaGetLastError());
    CHECK_CUDA_ERROR(cudaMemcpy(inGradB.data, d_inGradB, size, cudaMemcpyDeviceToHost));
    
    CHECK_CUDA_ERROR(cudaFree(d_outGrad));
    CHECK_CUDA_ERROR(cudaFree(d_inGradB));
}




// 元素级乘法的反向传播CUDA实现
__global__ void mulBackwardKernel(const float* outGrad, const float* A, const float* B, 
                                 float* inGradA, float* inGradB, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        // 对于A: ∂L/∂A = ∂L/∂(A*B) * ∂(A*B)/∂A = ∂L/∂(A*B) * B
        inGradA[idx] = outGrad[idx] * B[idx];
        
        // 对于B: ∂L/∂B = ∂L/∂(A*B) * ∂(A*B)/∂B = ∂L/∂(A*B) * A
        inGradB[idx] = outGrad[idx] * A[idx];
    }
}

void mulBackwardCUDA(const Matrix_CU& outGrad, const Matrix_CU& A, const Matrix_CU& B, 
                    Matrix_CU& inGradA, Matrix_CU& inGradB) {
    assert(outGrad.row == A.row && outGrad.col == A.col);
    assert(A.row == B.row && A.col == B.col);
    assert(outGrad.row == inGradA.row && outGrad.col == inGradA.col);
    assert(outGrad.row == inGradB.row && outGrad.col == inGradB.col);
    
    const int rows = outGrad.row;
    const int cols = outGrad.col;
    const int totalElements = rows * cols;
    const size_t size = totalElements * sizeof(float);
    
    float *d_outGrad, *d_A, *d_B, *d_inGradA, *d_inGradB;
    
    CHECK_CUDA_ERROR(cudaMalloc(&d_outGrad, size));
    CHECK_CUDA_ERROR(cudaMalloc(&d_A, size));
    CHECK_CUDA_ERROR(cudaMalloc(&d_B, size));
    CHECK_CUDA_ERROR(cudaMalloc(&d_inGradA, size));
    CHECK_CUDA_ERROR(cudaMalloc(&d_inGradB, size));
    
    CHECK_CUDA_ERROR(cudaMemcpy(d_outGrad, outGrad.data, size, cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaMemcpy(d_A, A.data, size, cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaMemcpy(d_B, B.data, size, cudaMemcpyHostToDevice));
    
    int blockSize = 256;
    int numBlocks = (totalElements + blockSize - 1) / blockSize;
    
    mulBackwardKernel<<<numBlocks, blockSize>>>(d_outGrad, d_A, d_B, d_inGradA, d_inGradB, totalElements);
    
    CHECK_CUDA_ERROR(cudaGetLastError());
    
    CHECK_CUDA_ERROR(cudaMemcpy(inGradA.data, d_inGradA, size, cudaMemcpyDeviceToHost));
    CHECK_CUDA_ERROR(cudaMemcpy(inGradB.data, d_inGradB, size, cudaMemcpyDeviceToHost));
    
    CHECK_CUDA_ERROR(cudaFree(d_outGrad));
    CHECK_CUDA_ERROR(cudaFree(d_A));
    CHECK_CUDA_ERROR(cudaFree(d_B));
    CHECK_CUDA_ERROR(cudaFree(d_inGradA));
    CHECK_CUDA_ERROR(cudaFree(d_inGradB));
}

// 元素级除法的反向传播CUDA实现
__global__ void divBackwardKernel(const float* outGrad, const float* A, const float* B, 
                                float* inGradA, float* inGradB, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        // 对于A: ∂L/∂A = ∂L/∂(A/B) * ∂(A/B)/∂A = ∂L/∂(A/B) * (1/B)
        inGradA[idx] = (B[idx] != 0.0f) ? (outGrad[idx] / B[idx]) : 0.0f;
        
        // 对于B: ∂L/∂B = ∂L/∂(A/B) * ∂(A/B)/∂B = ∂L/∂(A/B) * (-A/B²)
        inGradB[idx] = (B[idx] != 0.0f) ? (outGrad[idx] * (-A[idx] / (B[idx] * B[idx]))) : 0.0f;
    }
}

void divBackwardCUDA(const Matrix_CU& outGrad, const Matrix_CU& A, const Matrix_CU& B, 
                   Matrix_CU& inGradA, Matrix_CU& inGradB) {
    // 与乘法类似，但导数计算不同
    assert(outGrad.row == A.row && outGrad.col == A.col);
    assert(A.row == B.row && A.col == B.col);
    assert(outGrad.row == inGradA.row && outGrad.col == inGradA.col);
    assert(outGrad.row == inGradB.row && outGrad.col == inGradB.col);
    
    const int rows = outGrad.row;
    const int cols = outGrad.col;
    const int totalElements = rows * cols;
    const size_t size = totalElements * sizeof(float);
    
    float *d_outGrad, *d_A, *d_B, *d_inGradA, *d_inGradB;
    
    CHECK_CUDA_ERROR(cudaMalloc(&d_outGrad, size));
    CHECK_CUDA_ERROR(cudaMalloc(&d_A, size));
    CHECK_CUDA_ERROR(cudaMalloc(&d_B, size));
    CHECK_CUDA_ERROR(cudaMalloc(&d_inGradA, size));
    CHECK_CUDA_ERROR(cudaMalloc(&d_inGradB, size));
    
    CHECK_CUDA_ERROR(cudaMemcpy(d_outGrad, outGrad.data, size, cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaMemcpy(d_A, A.data, size, cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaMemcpy(d_B, B.data, size, cudaMemcpyHostToDevice));
    
    int blockSize = 256;
    int numBlocks = (totalElements + blockSize - 1) / blockSize;
    
    divBackwardKernel<<<numBlocks, blockSize>>>(d_outGrad, d_A, d_B, d_inGradA, d_inGradB, totalElements);
    
    CHECK_CUDA_ERROR(cudaGetLastError());
    
    CHECK_CUDA_ERROR(cudaMemcpy(inGradA.data, d_inGradA, size, cudaMemcpyDeviceToHost));
    CHECK_CUDA_ERROR(cudaMemcpy(inGradB.data, d_inGradB, size, cudaMemcpyDeviceToHost));
    
    CHECK_CUDA_ERROR(cudaFree(d_outGrad));
    CHECK_CUDA_ERROR(cudaFree(d_A));
    CHECK_CUDA_ERROR(cudaFree(d_B));
    CHECK_CUDA_ERROR(cudaFree(d_inGradA));
    CHECK_CUDA_ERROR(cudaFree(d_inGradB));
}

// 计算图管理类
class ComputeGraph {
public:
    std::vector<ComputeNode*> nodes;
    
    ~ComputeGraph() {
        for (auto node : nodes) {
            delete node;
        }
    }
    
    // 添加输入节点
    ComputeNode* addInput(int rows, int cols, int batchSize = 1) {
        ComputeNode* node = new ComputeNode(rows, cols, batchSize);
        node->op = OP_INPUT;
        nodes.push_back(node);
        return node;
    }
    
    // 添加加法节点
    ComputeNode* addAdd(ComputeNode* a, ComputeNode* b) {
        assert(a->value.row == b->value.row && a->value.col == b->value.col);
        
        ComputeNode* node = new ComputeNode(a->value.row / a->batchSize, a->value.col, a->batchSize);
        node->op = OP_ADD;
        node->inputs.push_back(a);
        node->inputs.push_back(b);
        nodes.push_back(node);
        return node;
    }
    
    // 添加减法节点
    ComputeNode* addSub(ComputeNode* a, ComputeNode* b) {
        assert(a->value.row == b->value.row && a->value.col == b->value.col);
        
        ComputeNode* node = new ComputeNode(a->value.row / a->batchSize, a->value.col, a->batchSize);
        node->op = OP_SUB;
        node->inputs.push_back(a);
        node->inputs.push_back(b);
        nodes.push_back(node);
        return node;
    }
    
    // 添加元素级乘法节点
    ComputeNode* addMul(ComputeNode* a, ComputeNode* b) {
        assert(a->value.row == b->value.row && a->value.col == b->value.col);
        
        ComputeNode* node = new ComputeNode(a->value.row / a->batchSize, a->value.col, a->batchSize);
        node->op = OP_MUL;
        node->inputs.push_back(a);
        node->inputs.push_back(b);
        nodes.push_back(node);
        return node;
    }
    
    // 添加元素级除法节点
    ComputeNode* addDiv(ComputeNode* a, ComputeNode* b) {
        assert(a->value.row == b->value.row && a->value.col == b->value.col);
        
        ComputeNode* node = new ComputeNode(a->value.row / a->batchSize, a->value.col, a->batchSize);
        node->op = OP_DIV;
        node->inputs.push_back(a);
        node->inputs.push_back(b);
        nodes.push_back(node);
        return node;
    }
    
    // 添加矩阵乘法节点
    ComputeNode* addMatMul(ComputeNode* a, ComputeNode* b) {
        assert(a->value.col == b->value.row);
        
        ComputeNode* node = new ComputeNode(a->value.row / a->batchSize, b->value.col, a->batchSize);
        node->op = OP_MATMUL;
        node->inputs.push_back(a);
        node->inputs.push_back(b);
        nodes.push_back(node);
        return node;
    }
    
    // 添加ReLU激活函数节点
    ComputeNode* addReLU(ComputeNode* input) {
        ComputeNode* node = new ComputeNode(input->value.row / input->batchSize, input->value.col, input->batchSize);
        node->op = OP_RELU;
        node->inputs.push_back(input);
        nodes.push_back(node);
        return node;
    }
    
    // 添加Sigmoid激活函数节点
    ComputeNode* addSigmoid(ComputeNode* input) {
        ComputeNode* node = new ComputeNode(input->value.row / input->batchSize, input->value.col, input->batchSize);
        node->op = OP_SIGMOID;
        node->inputs.push_back(input);
        nodes.push_back(node);
        return node;
    }
    ComputeNode* addConv2D(ComputeNode* input, ComputeNode* kernel, 
                         int in_height, int in_width, int in_channels,
                         int stride_height = 1, int stride_width = 1, 
                         int pad_height = 0, int pad_width = 0) {
        
        int batch_size = input->batchSize;
        int out_channels = kernel->value.row;
        
        // Calculate kernel dimensions
        int kernel_height, kernel_width;
        kernel_height = kernel_width = (int)sqrt(kernel->value.col / in_channels);
        
        // Calculate output dimensions
        int out_height = ((in_height + 2 * pad_height - kernel_height) / stride_height) + 1;
        int out_width = ((in_width + 2 * pad_width - kernel_width) / stride_width) + 1;
        
        // Create output node - result will be [batch_size * out_channels, out_height * out_width]
        ComputeNode* node = new ComputeNode(out_channels, out_height * out_width, batch_size);
        node->op = OP_CONV2D;
        node->inputs.push_back(input);
        node->inputs.push_back(kernel);
        
        // Store convolution parameters
        node->conv_params.in_channels = in_channels;
        node->conv_params.out_channels = out_channels;
        node->conv_params.kernel_height = kernel_height;
        node->conv_params.kernel_width = kernel_width;
        node->conv_params.stride_height = stride_height;
        node->conv_params.stride_width = stride_width;
        node->conv_params.pad_height = pad_height;
        node->conv_params.pad_width = pad_width;
        node->conv_params.in_height = in_height;
        node->conv_params.in_width = in_width;
        node->conv_params.out_height = out_height;
        node->conv_params.out_width = out_width;
        node->conv_params.batch_size = batch_size;
        
        nodes.push_back(node);
        return node;
    }
    // 前向传播
    void forward() {
        for (ComputeNode* node : nodes) {
            switch (node->op) {
                case OP_CONV2D:
                    // Implement convolution forward pass
                    {
                        const ConvParams& params = node->conv_params;
                        
                        // Reshape input from [batch_size * in_channels, in_height * in_width] 
                        // to [batch_size, in_channels, in_height, in_width]
                        std::vector<float> input_reshaped(params.batch_size * params.in_channels * 
                                                        params.in_height * params.in_width);
                        
                        // Reshape kernel from [out_channels, in_channels * kernel_height * kernel_width]
                        // to [out_channels, in_channels, kernel_height, kernel_width]
                        std::vector<float> kernel_reshaped(params.out_channels * params.in_channels * 
                                                         params.kernel_height * params.kernel_width);
                        
                        // Manual reshape for input
                        for (int n = 0; n < params.batch_size; n++) {
                            for (int c = 0; c < params.in_channels; c++) {
                                for (int h = 0; h < params.in_height; h++) {
                                    for (int w = 0; w < params.in_width; w++) {
                                        int flat_idx = (n * params.in_channels + c) * (params.in_height * params.in_width) + 
                                                     (h * params.in_width + w);
                                        int tensor_idx = ((n * params.in_channels + c) * params.in_height + h) * 
                                                       params.in_width + w;
                                        input_reshaped[tensor_idx] = node->inputs[0]->value.data[flat_idx];
                                    }
                                }
                            }
                        }
                        
                        // Manual reshape for kernel
                        for (int oc = 0; oc < params.out_channels; oc++) {
                            for (int ic = 0; ic < params.in_channels; ic++) {
                                for (int kh = 0; kh < params.kernel_height; kh++) {
                                    for (int kw = 0; kw < params.kernel_width; kw++) {
                                        int flat_idx = oc * (params.in_channels * params.kernel_height * params.kernel_width) + 
                                                     (ic * params.kernel_height * params.kernel_width + 
                                                      kh * params.kernel_width + kw);
                                        int tensor_idx = ((oc * params.in_channels + ic) * params.kernel_height + kh) * 
                                                       params.kernel_width + kw;
                                        kernel_reshaped[tensor_idx] = node->inputs[1]->value.data[flat_idx];
                                    }
                                }
                            }
                        }
                        
                        // Output tensor
                        std::vector<float> output_tensor(params.batch_size * params.out_channels * 
                                                      params.out_height * params.out_width, 0.0f);
                        
                        // Allocate device memory
                        float *d_input, *d_kernel, *d_output;
                        size_t input_size = input_reshaped.size() * sizeof(float);
                        size_t kernel_size = kernel_reshaped.size() * sizeof(float);
                        size_t output_size = output_tensor.size() * sizeof(float);
                        
                        CHECK_CUDA_ERROR(cudaMalloc(&d_input, input_size));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_kernel, kernel_size));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_output, output_size));
                        
                        CHECK_CUDA_ERROR(cudaMemcpy(d_input, input_reshaped.data(), input_size, cudaMemcpyHostToDevice));
                        CHECK_CUDA_ERROR(cudaMemcpy(d_kernel, kernel_reshaped.data(), kernel_size, cudaMemcpyHostToDevice));
                        
                        // Launch kernel
                        dim3 gridSize(params.batch_size, params.out_channels, params.out_height * params.out_width);
                        dim3 blockSize(1, 1, 1);  // Simple configuration
                        
                        conv2dForwardKernel<<<gridSize, blockSize>>>(
                            d_input, d_kernel, d_output,
                            params.batch_size, params.in_channels, params.out_channels,
                            params.in_height, params.in_width, params.kernel_height, params.kernel_width,
                            params.out_height, params.out_width, params.stride_height, params.stride_width,
                            params.pad_height, params.pad_width
                        );
                        
                        CHECK_CUDA_ERROR(cudaGetLastError());
                        CHECK_CUDA_ERROR(cudaMemcpy(output_tensor.data(), d_output, output_size, cudaMemcpyDeviceToHost));
                        
                        // Reshape output back to matrix format
                        for (int n = 0; n < params.batch_size; n++) {
                            for (int c = 0; c < params.out_channels; c++) {
                                for (int h = 0; h < params.out_height; h++) {
                                    for (int w = 0; w < params.out_width; w++) {
                                        int tensor_idx = ((n * params.out_channels + c) * params.out_height + h) * 
                                                       params.out_width + w;
                                        int flat_idx = (n * params.out_channels + c) * (params.out_height * params.out_width) + 
                                                     (h * params.out_width + w);
                                        node->value.data[flat_idx] = output_tensor[tensor_idx];
                                    }
                                }
                            }
                        }
                        
                        CHECK_CUDA_ERROR(cudaFree(d_input));
                        CHECK_CUDA_ERROR(cudaFree(d_kernel));
                        CHECK_CUDA_ERROR(cudaFree(d_output));
                    }
                    break;
                case OP_INPUT:
                    // 输入节点不需要计算
                    break;
                    
                case OP_ADD:
                    matrixAddCUDA(node->inputs[0]->value, node->inputs[1]->value, node->value);
                    break;
                    
                case OP_SUB:
                    matrixSubCUDA(node->inputs[0]->value, node->inputs[1]->value, node->value);
                    break;
                    
                case OP_MUL:
                    matrixMulCUDA(node->inputs[0]->value, node->inputs[1]->value, node->value);
                    break;
                    
                case OP_DIV:
                    matrixDivCUDA(node->inputs[0]->value, node->inputs[1]->value, node->value);
                    break;
                
                case OP_MATMUL:
                    // 实现矩阵乘法前向传播
                    {
                        const int A_rows = node->inputs[0]->value.row;
                        const int A_cols = node->inputs[0]->value.col;
                        const int B_cols = node->inputs[1]->value.col;
                        size_t A_size = A_rows * A_cols * sizeof(float);
                        size_t B_size = A_cols * B_cols * sizeof(float);
                        size_t C_size = A_rows * B_cols * sizeof(float);
                        
                        float *d_A, *d_B, *d_C;
                        CHECK_CUDA_ERROR(cudaMalloc(&d_A, A_size));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_B, B_size));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_C, C_size));
                        
                        CHECK_CUDA_ERROR(cudaMemcpy(d_A, node->inputs[0]->value.data, A_size, cudaMemcpyHostToDevice));
                        CHECK_CUDA_ERROR(cudaMemcpy(d_B, node->inputs[1]->value.data, B_size, cudaMemcpyHostToDevice));
                        
                        dim3 blockSize(16, 16);
                        dim3 gridSize((B_cols + blockSize.x - 1) / blockSize.x,
                                      (A_rows + blockSize.y - 1) / blockSize.y);
                        
                        matmulForwardKernel<<<gridSize, blockSize>>>(d_A, d_B, d_C, A_rows, A_cols, B_cols);
                        
                        CHECK_CUDA_ERROR(cudaGetLastError());
                        CHECK_CUDA_ERROR(cudaMemcpy(node->value.data, d_C, C_size, cudaMemcpyDeviceToHost));
                        
                        CHECK_CUDA_ERROR(cudaFree(d_A));
                        CHECK_CUDA_ERROR(cudaFree(d_B));
                        CHECK_CUDA_ERROR(cudaFree(d_C));
                    }
                    break;
                    
                case OP_RELU:
                    // 实现ReLU前向传播
                    {
                        const int size = node->value.row * node->value.col;
                        const size_t bytes = size * sizeof(float);
                        
                        float *d_input, *d_output;
                        CHECK_CUDA_ERROR(cudaMalloc(&d_input, bytes));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_output, bytes));
                        
                        CHECK_CUDA_ERROR(cudaMemcpy(d_input, node->inputs[0]->value.data, bytes, cudaMemcpyHostToDevice));
                        
                        int blockSize = 256;
                        int numBlocks = (size + blockSize - 1) / blockSize;
                        
                        reluForwardKernel<<<numBlocks, blockSize>>>(d_input, d_output, size);
                        
                        CHECK_CUDA_ERROR(cudaGetLastError());
                        CHECK_CUDA_ERROR(cudaMemcpy(node->value.data, d_output, bytes, cudaMemcpyDeviceToHost));
                        
                        CHECK_CUDA_ERROR(cudaFree(d_input));
                        CHECK_CUDA_ERROR(cudaFree(d_output));
                    }
                    break;
                    
                case OP_SIGMOID:
                    // 实现Sigmoid前向传播
                    {
                        const int size = node->value.row * node->value.col;
                        const size_t bytes = size * sizeof(float);
                        
                        float *d_input, *d_output;
                        CHECK_CUDA_ERROR(cudaMalloc(&d_input, bytes));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_output, bytes));
                        
                        CHECK_CUDA_ERROR(cudaMemcpy(d_input, node->inputs[0]->value.data, bytes, cudaMemcpyHostToDevice));
                        
                        int blockSize = 256;
                        int numBlocks = (size + blockSize - 1) / blockSize;
                        
                        sigmoidForwardKernel<<<numBlocks, blockSize>>>(d_input, d_output, size);
                        
                        CHECK_CUDA_ERROR(cudaGetLastError());
                        CHECK_CUDA_ERROR(cudaMemcpy(node->value.data, d_output, bytes, cudaMemcpyDeviceToHost));
                        
                        CHECK_CUDA_ERROR(cudaFree(d_input));
                        CHECK_CUDA_ERROR(cudaFree(d_output));
                    }
                    break;
                
                // 其他操作...
                default:
                    std::cerr << "Unsupported operation in forward pass." << std::endl;
                    break;
            }
        }
    }
    
    // 反向传播
    void backward(ComputeNode* outputNode) {
        // 重置所有节点的梯度
        for (ComputeNode* node : nodes) {
            node->zeroGrad();
        }
        
        // 输出节点的梯度初始化为1
        std::fill(outputNode->grad.data, outputNode->grad.data + outputNode->grad.row * outputNode->grad.col, 1.0f);
        
        // 从输出节点开始反向传播
        for (int i = nodes.size() - 1; i >= 0; --i) {
            ComputeNode* node = nodes[i];
            switch (node->op) {
                case OP_CONV2D:
                    {
                        const ConvParams& params = node->conv_params;
                        
                        // Reshape gradients and values for convolution backprop
                        std::vector<float> outGrad_reshaped(params.batch_size * params.out_channels * 
                                                         params.out_height * params.out_width);
                        std::vector<float> input_reshaped(params.batch_size * params.in_channels * 
                                                       params.in_height * params.in_width);
                        std::vector<float> kernel_reshaped(params.out_channels * params.in_channels * 
                                                        params.kernel_height * params.kernel_width);
                        
                        // Reshape output gradient
                        for (int n = 0; n < params.batch_size; n++) {
                            for (int c = 0; c < params.out_channels; c++) {
                                for (int h = 0; h < params.out_height; h++) {
                                    for (int w = 0; w < params.out_width; w++) {
                                        int flat_idx = (n * params.out_channels + c) * (params.out_height * params.out_width) + 
                                                     (h * params.out_width + w);
                                        int tensor_idx = ((n * params.out_channels + c) * params.out_height + h) * 
                                                       params.out_width + w;
                                        outGrad_reshaped[tensor_idx] = node->grad.data[flat_idx];
                                    }
                                }
                            }
                        }
                        
                        // Reshape input
                        for (int n = 0; n < params.batch_size; n++) {
                            for (int c = 0; c < params.in_channels; c++) {
                                for (int h = 0; h < params.in_height; h++) {
                                    for (int w = 0; w < params.in_width; w++) {
                                        int flat_idx = (n * params.in_channels + c) * (params.in_height * params.in_width) + 
                                                     (h * params.in_width + w);
                                        int tensor_idx = ((n * params.in_channels + c) * params.in_height + h) * 
                                                       params.in_width + w;
                                        input_reshaped[tensor_idx] = node->inputs[0]->value.data[flat_idx];
                                    }
                                }
                            }
                        }
                        
                        // Reshape kernel
                        for (int oc = 0; oc < params.out_channels; oc++) {
                            for (int ic = 0; ic < params.in_channels; ic++) {
                                for (int kh = 0; kh < params.kernel_height; kh++) {
                                    for (int kw = 0; kw < params.kernel_width; kw++) {
                                        int flat_idx = oc * (params.in_channels * params.kernel_height * params.kernel_width) + 
                                                     (ic * params.kernel_height * params.kernel_width + 
                                                      kh * params.kernel_width + kw);
                                        int tensor_idx = ((oc * params.in_channels + ic) * params.kernel_height + kh) * 
                                                       params.kernel_width + kw;
                                        kernel_reshaped[tensor_idx] = node->inputs[1]->value.data[flat_idx];
                                    }
                                }
                            }
                        }
                        
                        // Prepare gradient outputs
                        std::vector<float> inGrad_reshaped(params.batch_size * params.in_channels * 
                                                        params.in_height * params.in_width, 0.0f);
                        std::vector<float> kernelGrad_reshaped(params.out_channels * params.in_channels * 
                                                            params.kernel_height * params.kernel_width, 0.0f);
                        
                        // Allocate device memory
                        float *d_outGrad, *d_input, *d_kernel, *d_inGrad, *d_kernelGrad;
                        size_t outGrad_size = outGrad_reshaped.size() * sizeof(float);
                        size_t input_size = input_reshaped.size() * sizeof(float);
                        size_t kernel_size = kernel_reshaped.size() * sizeof(float);
                        size_t inGrad_size = inGrad_reshaped.size() * sizeof(float);
                        size_t kernelGrad_size = kernelGrad_reshaped.size() * sizeof(float);
                        
                        CHECK_CUDA_ERROR(cudaMalloc(&d_outGrad, outGrad_size));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_input, input_size));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_kernel, kernel_size));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_inGrad, inGrad_size));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_kernelGrad, kernelGrad_size));
                        
                        CHECK_CUDA_ERROR(cudaMemcpy(d_outGrad, outGrad_reshaped.data(), outGrad_size, cudaMemcpyHostToDevice));
                        CHECK_CUDA_ERROR(cudaMemcpy(d_input, input_reshaped.data(), input_size, cudaMemcpyHostToDevice));
                        CHECK_CUDA_ERROR(cudaMemcpy(d_kernel, kernel_reshaped.data(), kernel_size, cudaMemcpyHostToDevice));
                        CHECK_CUDA_ERROR(cudaMemcpy(d_inGrad, inGrad_reshaped.data(), inGrad_size, cudaMemcpyHostToDevice));
                        
                        // Launch input gradient kernel
                        dim3 gridSizeInput(params.batch_size, params.in_channels, params.in_height * params.in_width);
                        dim3 blockSizeInput(1, 1, 1);
                        
                        conv2dBackwardInputKernel<<<gridSizeInput, blockSizeInput>>>(
                            d_outGrad, d_kernel, d_inGrad,
                            params.batch_size, params.in_channels, params.out_channels,
                            params.in_height, params.in_width, params.kernel_height, params.kernel_width,
                            params.out_height, params.out_width, params.stride_height, params.stride_width,
                            params.pad_height, params.pad_width
                        );
                        
                        // Launch kernel gradient kernel
                        dim3 gridSizeKernel(params.out_channels, params.in_channels, 
                                          params.kernel_height * params.kernel_width);
                        dim3 blockSizeKernel(1, 1, 1);
                        
                        conv2dBackwardKernelKernel<<<gridSizeKernel, blockSizeKernel>>>(
                            d_input, d_outGrad, d_kernelGrad,
                            params.batch_size, params.in_channels, params.out_channels,
                            params.in_height, params.in_width, params.kernel_height, params.kernel_width,
                            params.out_height, params.out_width, params.stride_height, params.stride_width,
                            params.pad_height, params.pad_width
                        );
                        
                        CHECK_CUDA_ERROR(cudaGetLastError());
                        
                        CHECK_CUDA_ERROR(cudaMemcpy(inGrad_reshaped.data(), d_inGrad, inGrad_size, cudaMemcpyDeviceToHost));
                        CHECK_CUDA_ERROR(cudaMemcpy(kernelGrad_reshaped.data(), d_kernelGrad, kernelGrad_size, cudaMemcpyDeviceToHost));
                        
                        // Reshape gradients back to matrix format
                        for (int n = 0; n < params.batch_size; n++) {
                            for (int c = 0; c < params.in_channels; c++) {
                                for (int h = 0; h < params.in_height; h++) {
                                    for (int w = 0; w < params.in_width; w++) {
                                        int tensor_idx = ((n * params.in_channels + c) * params.in_height + h) * 
                                                       params.in_width + w;
                                        int flat_idx = (n * params.in_channels + c) * (params.in_height * params.in_width) + 
                                                     (h * params.in_width + w);
                                        node->inputs[0]->grad.data[flat_idx] += inGrad_reshaped[tensor_idx];
                                    }
                                }
                            }
                        }
                        
                        for (int oc = 0; oc < params.out_channels; oc++) {
                            for (int ic = 0; ic < params.in_channels; ic++) {
                                for (int kh = 0; kh < params.kernel_height; kh++) {
                                    for (int kw = 0; kw < params.kernel_width; kw++) {
                                        int tensor_idx = ((oc * params.in_channels + ic) * params.kernel_height + kh) * 
                                                       params.kernel_width + kw;
                                        int flat_idx = oc * (params.in_channels * params.kernel_height * params.kernel_width) + 
                                                     (ic * params.kernel_height * params.kernel_width + 
                                                      kh * params.kernel_width + kw);
                                        node->inputs[1]->grad.data[flat_idx] += kernelGrad_reshaped[tensor_idx];
                                    }
                                }
                            }
                        }
                        
                        CHECK_CUDA_ERROR(cudaFree(d_outGrad));
                        CHECK_CUDA_ERROR(cudaFree(d_input));
                        CHECK_CUDA_ERROR(cudaFree(d_kernel));
                        CHECK_CUDA_ERROR(cudaFree(d_inGrad));
                        CHECK_CUDA_ERROR(cudaFree(d_kernelGrad));
                    }
                    break;
                case OP_INPUT:
                    // 输入节点是梯度累积的终点
                    break;
                    
                case OP_ADD:
                    // 加法反向传播
                    addBackwardCUDA(node->grad, node->inputs[0]->grad, node->inputs[1]->grad);
                    break;
                    
                case OP_SUB:
                    // 减法反向传播
                    subBackwardCUDA(node->grad, node->inputs[0]->grad, node->inputs[1]->grad);
                    break;
                    
                case OP_MUL:
                    // 乘法反向传播
                    mulBackwardCUDA(node->grad, node->inputs[0]->value, node->inputs[1]->value, 
                                  node->inputs[0]->grad, node->inputs[1]->grad);
                    break;
                    
                case OP_DIV:
                    // 除法反向传播
                    divBackwardCUDA(node->grad, node->inputs[0]->value, node->inputs[1]->value, 
                                  node->inputs[0]->grad, node->inputs[1]->grad);
                    break;
                
                case OP_MATMUL:
                    // 矩阵乘法反向传播
                    {
                        const int A_rows = node->inputs[0]->value.row;
                        const int A_cols = node->inputs[0]->value.col;
                        const int B_rows = node->inputs[1]->value.row;
                        const int B_cols = node->inputs[1]->value.col;
                        const int outGrad_rows = node->grad.row;
                        const int outGrad_cols = node->grad.col;
                        
                        // A的梯度计算
                        float *d_outGrad, *d_B, *d_inGradA;
                        size_t outGrad_size = outGrad_rows * outGrad_cols * sizeof(float);
                        size_t B_size = B_rows * B_cols * sizeof(float);
                        size_t inGradA_size = A_rows * A_cols * sizeof(float);
                        
                        CHECK_CUDA_ERROR(cudaMalloc(&d_outGrad, outGrad_size));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_B, B_size));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_inGradA, inGradA_size));
                        
                        CHECK_CUDA_ERROR(cudaMemcpy(d_outGrad, node->grad.data, outGrad_size, cudaMemcpyHostToDevice));
                        CHECK_CUDA_ERROR(cudaMemcpy(d_B, node->inputs[1]->value.data, B_size, cudaMemcpyHostToDevice));
                        
                        dim3 blockSizeA(16, 16);
                        dim3 gridSizeA((A_cols + blockSizeA.x - 1) / blockSizeA.x,
                                      (A_rows + blockSizeA.y - 1) / blockSizeA.y);
                        
                        matmulBackwardAKernel<<<gridSizeA, blockSizeA>>>(d_outGrad, d_B, d_inGradA, 
                                                                       outGrad_rows, B_rows, B_cols);
                        
                        CHECK_CUDA_ERROR(cudaGetLastError());
                        CHECK_CUDA_ERROR(cudaMemcpy(node->inputs[0]->grad.data, d_inGradA, inGradA_size, cudaMemcpyDeviceToHost));
                        
                        CHECK_CUDA_ERROR(cudaFree(d_outGrad));
                        CHECK_CUDA_ERROR(cudaFree(d_B));
                        CHECK_CUDA_ERROR(cudaFree(d_inGradA));
                        
                        // B的梯度计算
                        float *d_A, *d_inGradB;
                        size_t A_size = A_rows * A_cols * sizeof(float);
                        size_t inGradB_size = B_rows * B_cols * sizeof(float);
                        
                        CHECK_CUDA_ERROR(cudaMalloc(&d_outGrad, outGrad_size));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_A, A_size));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_inGradB, inGradB_size));
                        
                        CHECK_CUDA_ERROR(cudaMemcpy(d_outGrad, node->grad.data, outGrad_size, cudaMemcpyHostToDevice));
                        CHECK_CUDA_ERROR(cudaMemcpy(d_A, node->inputs[0]->value.data, A_size, cudaMemcpyHostToDevice));
                        
                        dim3 blockSizeB(16, 16);
                        dim3 gridSizeB((B_cols + blockSizeB.x - 1) / blockSizeB.x,
                                      (B_rows + blockSizeB.y - 1) / blockSizeB.y);
                        
                        matmulBackwardBKernel<<<gridSizeB, blockSizeB>>>(d_A, d_outGrad, d_inGradB, 
                                                                       A_rows, A_cols, outGrad_cols);
                        
                        CHECK_CUDA_ERROR(cudaGetLastError());
                        CHECK_CUDA_ERROR(cudaMemcpy(node->inputs[1]->grad.data, d_inGradB, inGradB_size, cudaMemcpyDeviceToHost));
                        
                        CHECK_CUDA_ERROR(cudaFree(d_outGrad));
                        CHECK_CUDA_ERROR(cudaFree(d_A));
                        CHECK_CUDA_ERROR(cudaFree(d_inGradB));
                    }
                    break;
                    
                case OP_RELU:
                    // ReLU反向传播
                    {
                        const int size = node->grad.row * node->grad.col;
                        const size_t bytes = size * sizeof(float);
                        
                        float *d_outGrad, *d_input, *d_inGrad;
                        CHECK_CUDA_ERROR(cudaMalloc(&d_outGrad, bytes));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_input, bytes));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_inGrad, bytes));
                        
                        CHECK_CUDA_ERROR(cudaMemcpy(d_outGrad, node->grad.data, bytes, cudaMemcpyHostToDevice));
                        CHECK_CUDA_ERROR(cudaMemcpy(d_input, node->inputs[0]->value.data, bytes, cudaMemcpyHostToDevice));
                        
                        int blockSize = 256;
                        int numBlocks = (size + blockSize - 1) / blockSize;
                        
                        reluBackwardKernel<<<numBlocks, blockSize>>>(d_outGrad, d_input, d_inGrad, size);
                        
                        CHECK_CUDA_ERROR(cudaGetLastError());
                        CHECK_CUDA_ERROR(cudaMemcpy(node->inputs[0]->grad.data, d_inGrad, bytes, cudaMemcpyDeviceToHost));
                        
                        CHECK_CUDA_ERROR(cudaFree(d_outGrad));
                        CHECK_CUDA_ERROR(cudaFree(d_input));
                        CHECK_CUDA_ERROR(cudaFree(d_inGrad));
                    }
                    break;
                    
                case OP_SIGMOID:
                    // Sigmoid反向传播
                    {
                        const int size = node->grad.row * node->grad.col;
                        const size_t bytes = size * sizeof(float);
                        
                        float *d_outGrad, *d_output, *d_inGrad;
                        CHECK_CUDA_ERROR(cudaMalloc(&d_outGrad, bytes));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_output, bytes));
                        CHECK_CUDA_ERROR(cudaMalloc(&d_inGrad, bytes));
                        
                        CHECK_CUDA_ERROR(cudaMemcpy(d_outGrad, node->grad.data, bytes, cudaMemcpyHostToDevice));
                        CHECK_CUDA_ERROR(cudaMemcpy(d_output, node->value.data, bytes, cudaMemcpyHostToDevice));
                        
                        int blockSize = 256;
                        int numBlocks = (size + blockSize - 1) / blockSize;
                        
                        sigmoidBackwardKernel<<<numBlocks, blockSize>>>(d_outGrad, d_output, d_inGrad, size);
                        
                        CHECK_CUDA_ERROR(cudaGetLastError());
                        CHECK_CUDA_ERROR(cudaMemcpy(node->inputs[0]->grad.data, d_inGrad, bytes, cudaMemcpyDeviceToHost));
                        
                        CHECK_CUDA_ERROR(cudaFree(d_outGrad));
                        CHECK_CUDA_ERROR(cudaFree(d_output));
                        CHECK_CUDA_ERROR(cudaFree(d_inGrad));
                    }
                    break;
                
                // 其他操作...
                default:
                    std::cerr << "Unsupported operation in backward pass." << std::endl;
                    break;
            }
        }
    }
};
// CPU version of matrix multiplication
void matrixMatMulCPU(const Matrix_CU& A, const Matrix_CU& B, Matrix_CU& C) {
    assert(A.col == B.row);
    assert(C.row == A.row && C.col == B.col);
    
    for (int i = 0; i < A.row; ++i) {
        for (int j = 0; j < B.col; ++j) {
            C.data[i * C.col + j] = 0.0f;
            for (int k = 0; k < A.col; ++k) {
                C.data[i * C.col + j] += A.data[i * A.col + k] * B.data[k * B.col + j];
            }
        }
    }
}

int test4fullconnect() {
    // 可配置参数
    int input_rows = 100;        
    int input_cols = 100;         
    int output_cols = 50;         
    int batch_size = 32;           
    bool use_relu = true;          
    int print_rows = 5;           
    int print_cols = 5;           
    bool verify_with_cpu = true; 
    cudaFree(0);
    std::cout << "====== Configuration ======\n";
    std::cout << "Input matrix: " << input_rows << " x " << input_cols << std::endl;
    std::cout << "Weight matrix: " << input_cols << " x " << output_cols << std::endl;
    std::cout << "Batch size: " << batch_size << std::endl;
    std::cout << "Activation: " << (use_relu ? "ReLU" : "None") << std::endl;
    
    // 创建计算图
    ComputeGraph graph;
    
    ComputeNode* input = graph.addInput(input_rows, input_cols, batch_size);
    ComputeNode* weight = graph.addInput(input_cols, output_cols);
    
    input->value.randomInit();
    weight->value.randomInit();
    
    // 创建CPU版本的矩阵用于验证
    Matrix_CU input_cpu = input->value;
    Matrix_CU weight_cpu = weight->value;
    
    // 记录CUDA计算开始时间
    auto cuda_start = std::chrono::high_resolution_clock::now();
    
    // 构建前向计算图
    ComputeNode* fc1 = graph.addMatMul(input, weight);  // 全连接层
    ComputeNode* output_node = fc1;
    
    // 如果需要，添加ReLU激活
    if (use_relu) {
        output_node = graph.addReLU(fc1);
    }
    
    // 计算前向传播
    graph.forward();
    
    // 反向传播计算梯度
    graph.backward(output_node);
    
    // 记录CUDA计算结束时间
    auto cuda_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> cuda_duration = cuda_end - cuda_start;
    
    // 输出CUDA计算结果
    std::cout << "\n====== CUDA Results ======\n";
    std::cout << "Input first " << print_rows << "x" << print_cols << ":\n";
    input->value.printSubMatrix(print_rows, print_cols);
    
    std::cout << "Weight first " << print_rows << "x" << print_cols << ":\n";
    weight->value.printSubMatrix(std::min(print_rows, weight->value.row), 
                                std::min(print_cols, weight->value.col));
    
    std::cout << "Output first " << print_rows << "x" << print_cols << ":\n";
    output_node->value.printSubMatrix(print_rows, 
                                     std::min(print_cols, output_node->value.col));
    
    std::cout << "Input gradient first " << print_rows << "x" << print_cols << ":\n";
    input->grad.printSubMatrix(print_rows, print_cols);
    
    std::cout << "Weight gradient first " << print_rows << "x" << print_cols << ":\n";
    weight->grad.printSubMatrix(std::min(print_rows, weight->value.row), 
                               std::min(print_cols, weight->value.col));
    
    // 如果需要，使用CPU验证
    if (verify_with_cpu) {
        std::cout << "\n====== CPU Verification ======\n";
        
        // 记录CPU计算开始时间
        auto cpu_start = std::chrono::high_resolution_clock::now();
        
        // CPU版本的矩阵乘法计算 (FC1)
        Matrix_CU fc1_cpu(input_cpu.row, weight_cpu.col);
        matrixMatMulCPU(input_cpu, weight_cpu, fc1_cpu);
        
        // CPU版本输出节点
        Matrix_CU output_cpu = fc1_cpu;
        
        // 如果需要，CPU版本的ReLU计算
        if (use_relu) {
            output_cpu = Matrix_CU(fc1_cpu.row, fc1_cpu.col);
            for (int i = 0; i < fc1_cpu.row * fc1_cpu.col; ++i) {
                output_cpu.data[i] = fc1_cpu.data[i] > 0 ? fc1_cpu.data[i] : 0;
            }
        }
        
        // CPU版本的反向传播
        // 初始化输出梯度为1.0
        Matrix_CU output_grad(output_cpu.row, output_cpu.col);
        for (int i = 0; i < output_grad.row * output_grad.col; ++i) {
            output_grad.data[i] = 1.0f;
        }
        
        // ReLU反向传播
        Matrix_CU fc1_grad = output_grad;
        if (use_relu) {
            fc1_grad = Matrix_CU(fc1_cpu.row, fc1_cpu.col);
            for (int i = 0; i < fc1_cpu.row * fc1_cpu.col; ++i) {
                fc1_grad.data[i] = fc1_cpu.data[i] > 0 ? output_grad.data[i] : 0;
            }
        }
        
        // 矩阵乘法反向传播
        Matrix_CU input_grad_cpu(input_cpu.row, input_cpu.col);
        Matrix_CU weight_grad_cpu(weight_cpu.row, weight_cpu.col);
        
        // 计算输入梯度: input_grad = fc1_grad * weight^T
        Matrix_CU weight_transpose(weight_cpu.col, weight_cpu.row);
        for (int i = 0; i < weight_cpu.row; ++i) {
            for (int j = 0; j < weight_cpu.col; ++j) {
                weight_transpose.data[j * weight_cpu.row + i] = weight_cpu.data[i * weight_cpu.col + j];
            }
        }
        matrixMatMulCPU(fc1_grad, weight_transpose, input_grad_cpu);
        
        // 计算权重梯度: weight_grad = input^T * fc1_grad
        Matrix_CU input_transpose(input_cpu.col, input_cpu.row);
        for (int i = 0; i < input_cpu.row; ++i) {
            for (int j = 0; j < input_cpu.col; ++j) {
                input_transpose.data[j * input_cpu.row + i] = input_cpu.data[i * input_cpu.col + j];
            }
        }
        matrixMatMulCPU(input_transpose, fc1_grad, weight_grad_cpu);
        
        // 记录CPU计算结束时间
        auto cpu_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> cpu_duration = cpu_end - cpu_start;
        
        // 输出CPU计算结果
        std::cout << "CPU Output first " << print_rows << "x" << print_cols << ":\n";
        output_cpu.printSubMatrix(print_rows, std::min(print_cols, output_cpu.col));
        
        std::cout << "CPU Input gradient first " << print_rows << "x" << print_cols << ":\n";
        input_grad_cpu.printSubMatrix(print_rows, print_cols);
        
        std::cout << "CPU Weight gradient first " << print_rows << "x" << print_cols << ":\n";
        weight_grad_cpu.printSubMatrix(std::min(print_rows, weight_grad_cpu.row), 
                                      std::min(print_cols, weight_grad_cpu.col));
        
        // 验证CUDA和CPU结果的差异
        double output_diff = 0.0;
        double input_grad_diff = 0.0;
        double weight_grad_diff = 0.0;
        
        for (int i = 0; i < output_node->value.row * output_node->value.col; ++i) {
            output_diff += std::abs(output_node->value.data[i] - output_cpu.data[i]);
        }
        output_diff /= (output_node->value.row * output_node->value.col);
        
        for (int i = 0; i < input->grad.row * input->grad.col; ++i) {
            input_grad_diff += std::abs(input->grad.data[i] - input_grad_cpu.data[i]);
        }
        input_grad_diff /= (input->grad.row * input->grad.col);
        
        for (int i = 0; i < weight->grad.row * weight->grad.col; ++i) {
            weight_grad_diff += std::abs(weight->grad.data[i] - weight_grad_cpu.data[i]);
        }
        weight_grad_diff /= (weight->grad.row * weight->grad.col);
        
        // 输出结果比较和性能比较
        std::cout << "\n====== Verification Results ======\n";
        std::cout << "Average output difference: " << output_diff << std::endl;
        std::cout << "Average input gradient difference: " << input_grad_diff << std::endl;
        std::cout << "Average weight gradient difference: " << weight_grad_diff << std::endl;
        
        std::cout << "\n====== Performance Comparison ======\n";
        std::cout << "CUDA computation time: " << cuda_duration.count() << " ms" << std::endl;
        std::cout << "CPU computation time: " << cpu_duration.count() << " ms" << std::endl;
        std::cout << "Speedup: " << cpu_duration.count() / cuda_duration.count() << "x" << std::endl;
    } else {
        std::cout << "\n====== Performance ======\n";
        std::cout << "CUDA computation time: " << cuda_duration.count() << " ms" << std::endl;
    }
    
    return 0;
}