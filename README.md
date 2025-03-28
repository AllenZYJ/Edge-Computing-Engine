<div align=center><img src="picture/image-20200418210521131.png" alt="image-20200418210521131"  /></div>


# Edge-Computing-Engine

An open-source scientific computing engine designed for matrix computation and machine learning tasks, with CUDA acceleration support.

[![Edge support](https://img.shields.io/badge/SUPPORT-Macos-brightgreen)](https://support.apple.com/downloads/macos)
[![Edge support](https://img.shields.io/badge/SUPPORT-Ubuntu-brightgreen)](https://ubuntu.com/download/server)
[![Edge passing](https://img.shields.io/badge/Edge--CI-passing-blue)](https://github.com/AllenZYJ/Edge-Computing-Engine/actions)
[![GitHub license](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](https://raw.githubusercontent.com/tesseract-ocr/tesseract/master/LICENSE)

# Overview

Edge-Computing-Engine provides a flexible computational graph implementation that supports:

1. Forward propagation for neural network inference
2. Backward propagation for training via automatic differentiation
3. CUDA-accelerated operations for improved performance on NVIDIA GPUs

The computational graph is built using nodes representing operations such as matrix multiplication, element-wise operations, and activation functions.

# 1. GPU Features

### 1.1 Operation Types

The framework supports the following operations:

- **OP_INPUT**: Input nodes for data and parameters
- **OP_ADD**: Element-wise addition
- **OP_SUB**: Element-wise subtraction
- **OP_MUL**: Element-wise multiplication
- **OP_DIV**: Element-wise division
- **OP_MATMUL**: Matrix multiplication
- **OP_RELU**: ReLU activation function
- **OP_SIGMOID**: Sigmoid activation function
- **OP_TANH**: Tanh activation function (declared but not implemented)
- **OP_SOFTMAX**: Softmax function (declared but not implemented)

### 1.2 Core Classes

1. **Matrix_CU**: A custom matrix class with CUDA support
2. **ComputeNode**: Represents a node in the computational graph
3. **ComputeGraph**: Manages the graph, including node creation, forward and backward propagation

### 1.3 CUDA Kernels

The code includes several CUDA kernel functions:

- **reluForwardKernel/reluBackwardKernel**: Forward and backward propagation for ReLU
- **sigmoidForwardKernel/sigmoidBackwardKernel**: Forward and backward propagation for Sigmoid
- **matmulForwardKernel/matmulBackwardAKernel/matmulBackwardBKernel**: Forward and backward propagation for matrix multiplication
- **mulBackwardKernel**: Backward propagation for element-wise multiplication
- **divBackwardKernel**: Backward propagation for element-wise division
- **negGradKernel**: Helper kernel for computing negative gradients


### 1.4 CUDA Example

```cpp
// Create a computational graph
ComputeGraph graph;

// Create input and weight nodes
ComputeNode* input = graph.addInput(input_rows, input_cols, batch_size);
ComputeNode* weight = graph.addInput(input_cols, output_cols);

// Initialize with random values
input->value.randomInit();
weight->value.randomInit();

// Build forward computation graph
ComputeNode* fc1 = graph.addMatMul(input, weight);  // Fully connected layer
ComputeNode* output_node = graph.addReLU(fc1);      // Apply ReLU activation

// Perform forward propagation
graph.forward();

// Compute gradients via backward propagation
graph.backward(output_node);
```
### 1.5 GPU_Usage

```shell
root@5353:/backup# nvcc -arch=sm_80 -I/usr/local/cuda/include -L/usr/local/cuda/lib64 cuda_mat/mat_grad.cu -o main && ./main
root@5353:/backup# ./main
====== Configuration ======
Input matrix: 10000 x 1000
Weight matrix: 1000 x 500
Batch size: 32
Activation: ReLU

====== CUDA Results ======
Input first 5x5:
0.840188 0.394383 0.783099 0.79844 0.911647
0.45724 0.640368 0.717092 0.460067 0.54114
0.0860517 0.180886 0.426423 0.0470658 0.306902
0.0587587 0.0138811 0.622212 0.0391351 0.221551
0.456151 0.24875 0.0699601 0.742097 0.216935
Weight first 5x5:
0.817311 0.0247108 0.0146763 0.939293 0.502699
0.604392 0.921644 0.675689 0.948712 0.58803
0.824073 0.747934 0.0448163 0.757354 0.858343
0.308744 0.659798 0.941692 0.255731 0.539655
0.89383 0.609149 0.799556 0.726306 0.640965
Output first 5x5:
256.076 253.23 258.393 259.965 255.971
250.843 246.827 252.131 253.493 244.151
247.064 244.543 251.723 247.424 250.466
249.944 250.359 256.148 252.21 249.615
246.675 238.295 252.572 242.94 243.512
Input gradient first 5x5:
244.628 251.341 255.388 250.4 249.891
244.628 251.341 255.388 250.4 249.891
244.628 251.341 255.388 250.4 249.891
244.628 251.341 255.388 250.4 249.891
244.628 251.341 255.388 250.4 249.891
Weight gradient first 5x5:
159791 159791 159791 159791 159791
160010 160010 160010 160010 160010
160266 160266 160266 160266 160266
159899 159899 159899 159899 159899
159964 159964 159964 159964 159964

====== Performance ======
CUDA computation time: 2201.45 ms
```

### 1.6 Requirements

- CUDA-capable NVIDIA GPU (for GPU features)
- CUDA Toolkit
- C++11 or above
- nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2024 NVIDIA Corporation
Built on Thu_Mar_28_02:18:24_PDT_2024
Cuda compilation tools, release 12.4, V12.4.131
Build cuda_12.4.r12.4/compiler.34097967_0


# 2. CPU Features

- `read_csv(string &file_path)`: Reads a formatted file (csv) and returns a matrix with automatically calculated dimensions
- Formatted file writing interface (similar to `pandas.to_csv`)
- Broadcasting mechanism for matrices with padding interface
- Fully connected layer forward and backward propagation interfaces with automatic differentiation support
- Matrix differentiation and automatic differentiation interfaces
- Various matrix operations:
  - `create(row,cols)`: Creates a matrix with specified dimensions
  - `add`, `subtract`, `mul`: Basic matrix operations
  - `get_T`: Matrix transposition
  - `flatten`: Returns a flattened array
  - `matrix_rs`: Matrix structure compression
  - `matrix_sum`, `matrix_mean`: Statistical operations
  - `iloc`: Matrix slicing
  - And many more
- Activation functions (Relu)
- Loss functions (MSE)
- Neural network components (convolutional layers, pooling layers)
### 2.1 CPU_autodiff_Installation

```shell
git clone git@github.com:AllenZYJ/Edge-Computing-Engine.git
cd Edge-Computing-Engine/install_diff && make && make install
```

Note: If you're not a root user, you may need to add sudo to the make install command.

### 2.2 CPU_Usage

```shell
root@5353:/backup# .g++ main.cpp -o main -lautodiff
root@5353:/backup# ./main
0.000000+-0.000378*1.000000
-0.000378+0.000000*1.000000
....
-0.000378+0.000000*1.000000
0.000000+-0.000378*1.000000
-0.000378+0.000000*1.000000
-0.000378+0.000000*1.000000
weight_1_grad:
[
   -0.000378    -0.000378    -0.000378 
   -0.000378    -0.000378    -0.000378 
   -0.000378    -0.000378    -0.000378 
]
neraul end;
```

# 3. Contributing

Contributions are welcome. To contribute:

1. Fork the repository
2. Create a new branch: `git checkout -b my-new-feature`
3. Make your changes and commit them: `git commit -am 'Add some feature'`
4. Push your changes: `git push origin my-new-feature`
5. Submit a pull request

Please ensure your code adheres to the existing style and passes tests before submitting.

## Stargazers over time

[![Stargazers over time](https://starchart.cc/AllenZYJ/Edge-Computing-Engine.svg)](https://starchart.cc/AllenZYJ/Edge-Computing-Engine)

## Contact

- Email: zk@likedge.top or edge@ibooker.org.cn
- Website: [Likedge](http://likedge.top/)

## License

Edge-Computing-Engine is released under the Apache 2.0 License. See the [`LICENSE`](https://www.apache.org/licenses/LICENSE-2.0) file for details.