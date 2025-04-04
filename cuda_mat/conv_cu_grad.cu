// Test function for convolution
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cuda_runtime.h>
#include <cassert>
#include <cstring>
#include <cmath>
#include <functional>
#include "mat_grad.cu"
void testConvolution() {
    std::cout << "\n====== Convolution Test ======\n";
    
    // Configuration
    int batch_size = 2;
    int in_channels = 3;
    int in_height = 5;
    int in_width = 5;
    int out_channels = 2;
    int kernel_size = 3;
    int stride = 1;
    int padding = 1;
    
    // Expected output dimensions
    int out_height = ((in_height + 2 * padding - kernel_size) / stride) + 1;  // Should be 5
    int out_width = ((in_width + 2 * padding - kernel_size) / stride) + 1;    // Should be 5
    
    std::cout << "Configuration:\n";
    std::cout << "  Input: [" << batch_size << ", " << in_channels << ", " << in_height << ", " << in_width << "]\n";
    std::cout << "  Kernel: [" << out_channels << ", " << in_channels << ", " << kernel_size << ", " << kernel_size << "]\n";
    std::cout << "  Stride: [" << stride << ", " << stride << "], Padding: [" << padding << ", " << padding << "]\n";
    std::cout << "  Expected Output: [" << batch_size << ", " << out_channels << ", " << out_height << ", " << out_width << "]\n";
    
    ComputeGraph graph;
    
    // Create input tensor: [batch_size * in_channels, in_height * in_width]
    ComputeNode* input = graph.addInput(batch_size * in_channels, in_height * in_width);
    input->batchSize = batch_size;
    
    // Create kernel tensor: [out_channels, in_channels * kernel_size * kernel_size]
    ComputeNode* kernel = graph.addInput(out_channels, in_channels * kernel_size * kernel_size);
    
    // Initialize input with a pattern that increases along width, then height, then channel
    for (int b = 0; b < batch_size; b++) {
        for (int c = 0; c < in_channels; c++) {
            for (int h = 0; h < in_height; h++) {
                for (int w = 0; w < in_width; w++) {
                    int flat_idx = (b * in_channels + c) * (in_height * in_width) + (h * in_width + w);
                    // Simple pattern: value = (c+1) * 0.1 + h * 0.01 + w * 0.001
                    input->value.data[flat_idx] = (c + 1) * 0.1f + h * 0.01f + w * 0.001f;
                }
            }
        }
    }
    
    // Initialize kernel with identity-like pattern for testing
    for (int oc = 0; oc < out_channels; oc++) {
        for (int ic = 0; ic < in_channels; ic++) {
            for (int kh = 0; kh < kernel_size; kh++) {
                for (int kw = 0; kw < kernel_size; kw++) {
                    int flat_idx = oc * (in_channels * kernel_size * kernel_size) + 
                                 (ic * kernel_size * kernel_size + kh * kernel_size + kw);
                    
                    // Identity-like kernel for the center position, zeros elsewhere
                    // For convolution, this should preserve the input features with some edge effects
                    if (kh == kernel_size/2 && kw == kernel_size/2 && ic == oc % in_channels) {
                        kernel->value.data[flat_idx] = 1.0f;
                    } else {
                        kernel->value.data[flat_idx] = 0.0f;
                    }
                }
            }
        }
    }
    
    // Create convolution node
    ComputeNode* conv = graph.addConv2D(input, kernel, in_height, in_width, in_channels, 
                                      stride, stride, padding, padding);
    
    // Forward pass
    auto start = std::chrono::high_resolution_clock::now();
    graph.forward();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> forward_duration = end - start;
    
    std::cout << "\nForward Pass Results:\n";
    std::cout << "  Convolution output shape: [" << conv->value.row << ", " << conv->value.col << "]\n";
    std::cout << "  Time: " << forward_duration.count() << " ms\n";
    
    // Print sample of the output (first batch, first channel)
    std::cout << "\nOutput sample (first batch, first channel):\n";
    for (int h = 0; h < out_height; h++) {
        std::cout << "  ";
        for (int w = 0; w < out_width; w++) {
            int flat_idx = 0 * (out_height * out_width) + (h * out_width + w);
            printf("%.4f ", conv->value.data[flat_idx]);
        }
        std::cout << std::endl;
    }
    
    // Backward pass
    start = std::chrono::high_resolution_clock::now();
    graph.backward(conv);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> backward_duration = end - start;
    
    std::cout << "\nBackward Pass Results:\n";
    std::cout << "  Time: " << backward_duration.count() << " ms\n";
    
    // Print sample of input gradients (first batch, first channel)
    std::cout << "\nInput gradient sample (first batch, first channel):\n";
    for (int h = 0; h < 3; h++) {
        std::cout << "  ";
        for (int w = 0; w < 3; w++) {
            int flat_idx = 0 * (in_height * in_width) + (h * in_width + w);
            printf("%.4f ", input->grad.data[flat_idx]);
        }
        std::cout << std::endl;
    }
    
    // Print sample of kernel gradients (first output channel, first input channel)
    std::cout << "\nKernel gradient sample (first output channel, first input channel):\n";
    for (int kh = 0; kh < kernel_size; kh++) {
        std::cout << "  ";
        for (int kw = 0; kw < kernel_size; kw++) {
            int flat_idx = 0 * (in_channels * kernel_size * kernel_size) + 
                         (0 * kernel_size * kernel_size + kh * kernel_size + kw);
            printf("%.4f ", kernel->grad.data[flat_idx]);
        }
        std::cout << std::endl;
    }
    
    std::cout << "\nConvolution Test Completed\n";
}

// CPU reference implementation for verification
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

// Test function with CPU validation
void testConvolutionWithValidation() {
    std::cout << "\n====== Convolution Test with CPU Validation ======\n";
    
    // Configuration (smaller for CPU comparison)
    int batch_size = 2;
    int in_channels = 2;
    int in_height = 4;
    int in_width = 4;
    int out_channels = 2;
    int kernel_size = 3;
    int stride = 1;
    int padding = 1;
    
    // Expected output dimensions
    int out_height = ((in_height + 2 * padding - kernel_size) / stride) + 1;
    int out_width = ((in_width + 2 * padding - kernel_size) / stride) + 1;
    
    ComputeGraph graph;
    
    // Create input tensor
    ComputeNode* input = graph.addInput(batch_size * in_channels, in_height * in_width);
    input->batchSize = batch_size;
    
    // Create kernel tensor
    ComputeNode* kernel = graph.addInput(out_channels, in_channels * kernel_size * kernel_size);
    
    // Initialize with random values for better test
    for (int i = 0; i < input->value.row * input->value.col; i++) {
        input->value.data[i] = static_cast<float>(rand()) / RAND_MAX;
    }
    
    for (int i = 0; i < kernel->value.row * kernel->value.col; i++) {
        kernel->value.data[i] = static_cast<float>(rand()) / RAND_MAX * 0.1f;
    }
    
    // Create convolution node
    ComputeNode* conv = graph.addConv2D(input, kernel, in_height, in_width, in_channels, 
                                      stride, stride, padding, padding);
    
    // Forward pass
    graph.forward();
    
    // CPU reference calculation
    std::vector<float> input_reshaped(batch_size * in_channels * in_height * in_width);
    std::vector<float> kernel_reshaped(out_channels * in_channels * kernel_size * kernel_size);
    std::vector<float> output_cpu(batch_size * out_channels * out_height * out_width, 0.0f);
    
    // Reshape input for CPU calculation
    for (int n = 0; n < batch_size; n++) {
        for (int c = 0; c < in_channels; c++) {
            for (int h = 0; h < in_height; h++) {
                for (int w = 0; w < in_width; w++) {
                    int flat_idx = (n * in_channels + c) * (in_height * in_width) + (h * in_width + w);
                    int tensor_idx = ((n * in_channels + c) * in_height + h) * in_width + w;
                    input_reshaped[tensor_idx] = input->value.data[flat_idx];
                }
            }
        }
    }
    
    // Reshape kernel for CPU calculation
    for (int oc = 0; oc < out_channels; oc++) {
        for (int ic = 0; ic < in_channels; ic++) {
            for (int kh = 0; kh < kernel_size; kh++) {
                for (int kw = 0; kw < kernel_size; kw++) {
                    int flat_idx = oc * (in_channels * kernel_size * kernel_size) + 
                                 (ic * kernel_size * kernel_size + kh * kernel_size + kw);
                    int tensor_idx = ((oc * in_channels + ic) * kernel_size + kh) * kernel_size + kw;
                    kernel_reshaped[tensor_idx] = kernel->value.data[flat_idx];
                }
            }
        }
    }
    
    // CPU convolution
    conv2dCPU(input_reshaped.data(), kernel_reshaped.data(), output_cpu.data(),
             batch_size, in_channels, out_channels,
             in_height, in_width, kernel_size, kernel_size,
             out_height, out_width, stride, stride,
             padding, padding);
    
    // Compare results
    std::vector<float> output_gpu(batch_size * out_channels * out_height * out_width);
    for (int n = 0; n < batch_size; n++) {
        for (int c = 0; c < out_channels; c++) {
            for (int h = 0; h < out_height; h++) {
                for (int w = 0; w < out_width; w++) {
                    int flat_idx = (n * out_channels + c) * (out_height * out_width) + (h * out_width + w);
                    int tensor_idx = ((n * out_channels + c) * out_height + h) * out_width + w;
                    output_gpu[tensor_idx] = conv->value.data[flat_idx];
                }
            }
        }
    }
    
    // Calculate error
    float max_diff = 0.0f;
    float avg_diff = 0.0f;
    
    for (int i = 0; i < output_cpu.size(); i++) {
        float diff = std::abs(output_cpu[i] - output_gpu[i]);
        max_diff = std::max(max_diff, diff);
        avg_diff += diff;
    }
    avg_diff /= output_cpu.size();
    
    std::cout << "Validation Results:\n";
    std::cout << "  Maximum absolute difference: " << max_diff << std::endl;
    std::cout << "  Average absolute difference: " << avg_diff << std::endl;
    std::cout << "  Validation " << (max_diff < 1e-4 ? "PASSED" : "FAILED") << std::endl;
    
    std::cout << "\nConvolution Validation Completed\n";
}

// Update main function to test convolution
int main() {
    // Test convolution
    testConvolution();
    
    // Test with CPU validation
    testConvolutionWithValidation();
    
    // Original code can follow here
    // ...
    
    return 0;
}