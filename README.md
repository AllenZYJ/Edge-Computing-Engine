<!-- <div align=center><img src="./picture/01.svg"/></div> -->

<div align=center><img src="picture/image-20200418210521131.png" alt="image-20200418210521131"  /></div>



<div align=center><h1>Edge-Computing-Engine</h1></div>

<div align=center>一个开源的科学计算引擎,支持CUDA加速计算





Email: zk@likedge.top



[![Edge support](https://img.shields.io/badge/SUPPORT-Macos-brightgreen)](https://support.apple.com/downloads/macos)

[![Edge support](https://img.shields.io/badge/SUPPORT-Ubuntu-brightgreen)](https://ubuntu.com/download/server)

[![Edge passing](https://img.shields.io/badge/Edge--CI-passing-blue)](https://github.com/AllenZYJ/Edge-Computing-Engine/actions)

[![GitHub license](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](https://raw.githubusercontent.com/tesseract-ocr/tesseract/master/LICENSE)





</div>





**------**

Edge-Computing-Engine is an open-source scientific computing engine designed for matrix computation and machine learning tasks. It provides a wide range of matrix operations and neural network building blocks, now with powerful **CUDA** acceleration support.

## Overview

This code provides a flexible computational graph implementation that supports:

1. Forward propagation for neural network inference
2. Backward propagation for training via automatic differentiation
3. CUDA-accelerated operations for improved performance

The computational graph is built using nodes that represent operations such as matrix multiplication, element-wise operations, and activation functions. All operations are implemented with CUDA kernels for parallel execution on NVIDIA GPUs.



Email: zk@likedge.top

## GPU_Features

Operation Types

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

### Core Classes

1. **Matrix_CU**: A custom matrix class with CUDA support (defined in an external "matrix_cudadef.cu" file)
2. **ComputeNode**: Represents a node in the computational graph
3. **ComputeGraph**: Manages the graph, including node creation, forward and backward propagation

### CUDA Kernels

The code includes several CUDA kernel functions:

- **reluForwardKernel/reluBackwardKernel**: Forward and backward propagation for ReLU
- **sigmoidForwardKernel/sigmoidBackwardKernel**: Forward and backward propagation for Sigmoid
- **matmulForwardKernel/matmulBackwardAKernel/matmulBackwardBKernel**: Forward and backward propagation for matrix multiplication
- **mulBackwardKernel**: Backward propagation for element-wise multiplication
- **divBackwardKernel**: Backward propagation for element-wise division
- **negGradKernel**: Helper kernel for computing negative gradients

## Usage Example

The main function in the code demonstrates a practical usage example:

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

## Performance Comparison

The code includes an optional CPU verification step that:

1. Computes the same operations on CPU
2. Compares the results between CUDA and CPU implementations
3. Measures the performance difference between GPU and CPU execution

## Configuration Options

Configurable parameters in the main function:

- **input_rows**: Number of rows in the input matrix
- **input_cols**: Number of columns in the input matrix
- **output_cols**: Number of columns in the output matrix
- **batch_size**: Number of samples to process in parallel
- **use_relu**: Whether to use ReLU activation or not
- **print_rows/print_cols**: Number of rows/columns to print for verification
- **verify_with_cpu**: Whether to perform CPU verification

## Implementation Details

### Forward Propagation

For each operation type, the forward method:

1. Allocates memory on the GPU
2. Copies input data from host to GPU
3. Launches the appropriate CUDA kernel
4. Copies results back from GPU to host

### Backward Propagation

The backward method:

1. Sets output node gradient to 1.0
2. Propagates gradients backward through the computation graph
3. For each node, computes gradients with respect to its inputs using CUDA kernels
4. Accumulates gradients at input nodes

### Memory Management

The code carefully manages GPU memory by:

- Allocating GPU memory only when needed
- Freeing GPU memory after each operation
- Ensuring correct memory transfer between host and device

## Limitations and Potential Improvements

1. The current implementation transfers data between host and device for each operation, which could be optimized
2. Some operations like Tanh and Softmax are declared but not implemented
3. Memory management could be improved with CUDA streams and asynchronous operations
4. The implementation could benefit from cuBLAS for matrix operations

## Requirements

- CUDA-capable NVIDIA GPU
- CUDA Toolkit
- C++ compiler with C++11 support

------



**## CPU_Features**



\- `read_csv(string &file_path)`: Reads a formatted file (csv) and returns a matrix with automatically calculated dimensions.

\- Formatted file writing interface. Similar to `pandas.to_csv`.

\- Broadcasting mechanism for matrices with padding interface.

\- Fully connected layer forward and backward propagation interfaces with automatic differentiation support.

\- Matrix differentiation and automatic differentiation interfaces.

\- `save_txt(Matrix mid1,string path = "./",string delimiter = ",",string header="./")`: Reads the file header and writes formatted files. It supports writing matrix-type data, custom headers, writing file paths, and custom delimiters (default is ", ").

\- `create(row,cols)`: Creates a matrix with specified dimensions and initializes all elements to 0.

\- `move_ele(int &ele1, int &ele2)`: Changes the value of an element at a specific position.

\- `add(Matrix mid1, Matrix mid2, int flag = 1)`: Matrix addition operation with optional bitwise operation acceleration.

\- `subtract(Matrix mid1, Matrix mid2)`: Matrix subtraction operation.

\- `mul(Matrix mid1, Matrix mid2)`: Matrix multiplication operation.

\- `times_mat(int times,Matrix mid1)`: Scalar matrix multiplication.

\- `get_T(Matrix mid1)`: Matrix transposition operation.

\- `mul(matrix1,matrix2)`: Matrix product (complete mathematical definition).

\- `flatten(Matrix mid1)`: Returns a flattened array.

\- `matrix_rs(Matrix mid1,int rs_row,int rs_col)`: Matrix structure compression.

\- `matrix_sum(Matrix mid1)`: Matrix summation.

\- `matrix_mean(Matrix mid1)`: Matrix mean.

\- `apply(Matrix mid1,Matrix mid2,int axis = 0)`: Matrix concatenation.

\- `iloc(Matrix mid1,int start_x=0,int end_x=0,int start_y=0,int end_y=0)`: Matrix slicing.

\- `mul_simple(Matrix mid1,Matrix mid2)`: Element-wise matrix multiplication for machine learning applications.

\- `Relu`: Activation function matrix interface.

\- `MSE`: Mean squared error matrix interface.

\- Random weight matrix creation interface.

\- Convolutional neural network definition (including but not limited to convolution kernel, pooling layer definition, and custom loss interface).



**## Requirements**



\- C++11 or above.



**## Installation**



\- Clone the repository: `git clone git@github.com:AllenZYJ/Edge-Computing-Engine.git`

\- Build the project: `cd Edge-Computing-Engine/install_diff && make && make install`(if common user shuould add sudo in Makefile(in install_diff))



**## Usage**





\```shell

g++ main.cpp -o main -lautodiff

\```



\```shell

./main

\```



**## Contributing**



Contributions to Edge-Computing-Engine are welcome. To contribute, please follow these steps:



\- Fork the repository.

\- Create a new branch for your feature or bug fix: `git checkout -b my-new-feature`

\- Make your changes and commit them: `git commit -am 'Add some feature'`

\- Push your changes to the branch: `git push origin my-new-feature`

\- Submit a pull request.



Please ensure your code adheres to the existing style and passes the existing tests before submitting a pull request.



**## Stargazers over time**



[![Stargazers over time](https://starchart.cc/AllenZYJ/Edge-Computing-Engine.svg)](https://starchart.cc/AllenZYJ/Edge-Computing-Engine)



**## License**



email:zk@likedge.top | edge@ibooker.org.cn



The author's personal website is [Likedge](http://likedge.top/), and the author's email is zk@likedge.top.



Edge-Computing-Engine is released under the Apache2.0. See the `LICENSE` file for details.





<div align = center><img src = './picture/星月.svg'></div>