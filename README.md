# Edge-Computing-Engine

An open-source scientific computing engine designed for matrix computation and machine learning tasks, with CUDA acceleration support.

[![Edge support](https://img.shields.io/badge/SUPPORT-Macos-brightgreen)](https://support.apple.com/downloads/macos)
[![Edge support](https://img.shields.io/badge/SUPPORT-Ubuntu-brightgreen)](https://ubuntu.com/download/server)
[![Edge passing](https://img.shields.io/badge/Edge--CI-passing-blue)](https://github.com/AllenZYJ/Edge-Computing-Engine/actions)
[![GitHub license](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](https://raw.githubusercontent.com/tesseract-ocr/tesseract/master/LICENSE)

## Overview

Edge-Computing-Engine provides a flexible computational graph implementation that supports:

1. Forward propagation for neural network inference
2. Backward propagation for training via automatic differentiation
3. CUDA-accelerated operations for improved performance on NVIDIA GPUs

The computational graph is built using nodes representing operations such as matrix multiplication, element-wise operations, and activation functions.

## GPU Features

### Operation Types

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

1. **Matrix_CU**: A custom matrix class with CUDA support
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

## CPU Features

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

## Usage Example

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

## Requirements

- CUDA-capable NVIDIA GPU (for GPU features)
- CUDA Toolkit
- C++11 or above

## Installation

```shell
git clone git@github.com:AllenZYJ/Edge-Computing-Engine.git
cd Edge-Computing-Engine/install_diff && make && make install
```

Note: If you're not a root user, you may need to add sudo to the make install command.

## Usage

```shell
g++ main.cpp -o main -lautodiff
./main
```

## Performance

The code includes an optional CPU verification step that:
1. Computes the same operations on CPU
2. Compares the results between CUDA and CPU implementations
3. Measures the performance difference between GPU and CPU execution

## Contributing

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

Edge-Computing-Engine is released under the Apache 2.0 License. See the `LICENSE` file for details.