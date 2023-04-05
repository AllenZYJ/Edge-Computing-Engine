<div align=center><img src="picture/image-20200418210521131.png" alt="image-20200418210521131"  /></div>

<div align=center><h1>Edge-Computing-Engine</h1></div>


[![Edge support](https://img.shields.io/badge/SUPPORT-Macos-brightgreen)](https://support.apple.com/downloads/macos)
[![Edge support](https://img.shields.io/badge/SUPPORT-Ubuntu-brightgreen)](https://ubuntu.com/download/server)
[![Edge passing](https://img.shields.io/badge/Edge--CI-passing-blue)](https://github.com/AllenZYJ/Edge-Computing-Engine/actions)
[![GitHub license](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](https://raw.githubusercontent.com/tesseract-ocr/tesseract/master/LICENSE)

Edge-Computing-Engine is an open-source scientific computing engine designed for matrix computation and machine learning tasks. It provides a wide range of matrix operations and neural network building blocks.

Email: zk@likedge.top

## Features

- `read_csv(string &file_path)`: Reads a formatted file (csv) and returns a matrix with automatically calculated dimensions.
- Formatted file writing interface. Similar to `pandas.to_csv`.
- Broadcasting mechanism for matrices with padding interface.
- Fully connected layer forward and backward propagation interfaces with automatic differentiation support.
- Matrix differentiation and automatic differentiation interfaces.
- `save_txt(Matrix mid1,string path = "./",string delimiter = ",",string header="./")`: Reads the file header and writes formatted files. It supports writing matrix-type data, custom headers, writing file paths, and custom delimiters (default is ", ").
- `create(row,cols)`: Creates a matrix with specified dimensions and initializes all elements to 0.
- `move_ele(int &ele1, int &ele2)`: Changes the value of an element at a specific position.
- `add(Matrix mid1, Matrix mid2, int flag = 1)`: Matrix addition operation with optional bitwise operation acceleration.
- `subtract(Matrix mid1, Matrix mid2)`: Matrix subtraction operation.
- `mul(Matrix mid1, Matrix mid2)`: Matrix multiplication operation.
- `times_mat(int times,Matrix mid1)`: Scalar matrix multiplication.
- `get_T(Matrix mid1)`: Matrix transposition operation.
- `mul(matrix1,matrix2)`: Matrix product (complete mathematical definition).
- `flatten(Matrix mid1)`: Returns a flattened array.
- `matrix_rs(Matrix mid1,int rs_row,int rs_col)`: Matrix structure compression.
- `matrix_sum(Matrix mid1)`: Matrix summation.
- `matrix_mean(Matrix mid1)`: Matrix mean.
- `apply(Matrix mid1,Matrix mid2,int axis = 0)`: Matrix concatenation.
- `iloc(Matrix mid1,int start_x=0,int end_x=0,int start_y=0,int end_y=0)`: Matrix slicing.
- `mul_simple(Matrix mid1,Matrix mid2)`: Element-wise matrix multiplication for machine learning applications.
- `Relu`: Activation function matrix interface.
- `MSE`: Mean squared error matrix interface.
- Random weight matrix creation interface.
- Convolutional neural network definition (including but not limited to convolution kernel, pooling layer definition, and custom loss interface).

## Requirements

- C++11 or above.

## Installation

- Clone the repository: `git clone git@github.com:AllenZYJ/Edge-Computing-Engine.git`
- Build the project: `cd Edge-Computing-Engine/install_diff && make && make install`

## Usage


```shell
g++ main.cpp -o main -lautodiff
```

```shell
./main
```

## Contributing

Contributions to Edge-Computing-Engine are welcome. To contribute, please follow these steps:

- Fork the repository.
- Create a new branch for your feature or bug fix: `git checkout -b my-new-feature`
- Make your changes and commit them: `git commit -am 'Add some feature'`
- Push your changes to the branch: `git push origin my-new-feature`
- Submit a pull request.

Please ensure your code adheres to the existing style and passes the existing tests before submitting a pull request.

## License

email:zk@likedge.top | edge@ibooker.org.cn

The author's personal website is [Likedge](http://likedge.top/), and the author's email is zk@likedge.top.

Edge-Computing-Engine is released under the Apache2.0. See the `LICENSE` file for details.


<div align = center><img src = './picture/星月.svg'></div>

