class edge_layer
{
public:
    virtual ~edge_layer() {}
    virtual Matrix4d forward(Matrix4d mid1) = 0;
    virtual Matrix4d backward(Matrix4d grad_output) = 0;
    virtual int parameter_counter() = 0;
};

class conv2d : public edge_layer
{
private:
    Matrix4d input_cache;
    Matrix** kernel;

public:
    conv2d(Matrix4d mid_4, int in_channel, int out_channle, int _stride, int ksize, int _mode, int _padding)
    {
        mid4 = mid_4;
        input_dim = in_channel;
        output_channels = out_channle;
        stride = _stride;
        kernel_size = ksize;
        mode = _mode;
        padding = _padding;

        kernel = (Matrix**)malloc(input_dim * sizeof(Matrix*));
        for(int i = 0; i < input_dim; i++) {
            kernel[i] = (Matrix*)malloc(output_channels * sizeof(Matrix));
            for(int j = 0; j < output_channels; j++) {
                kernel[i][j] = CreateRandMat(kernel_size, kernel_size);
            }
        }
    }

    ~conv2d() {
        for(int i = 0; i < input_dim; i++) {
            for(int j = 0; j < output_channels; j++) {
                free_mat(kernel[i][j]);
            }
            free(kernel[i]);
        }
        free(kernel);
    }

    int arg1;
    Matrix4d mid4;
    int input_dim;
    int output_channels;
    int stride;
    int kernel_size;
    int mode;
    int padding;

    Matrix4d forward(Matrix4d mid4) override
    {
        input_cache = mid4;
        std::cout << "in_channel = " << input_dim << std::endl;
        std::cout << "out_channle = " << output_channels << std::endl;
        std::cout << "_stride = " << stride << std::endl;
        std::cout << "ksize = " << kernel_size << std::endl;
        std::cout << "_mode = " << mode << std::endl;
        std::cout << "_padding = " << padding << std::endl;
        
        Matrix3d *output3d_arr = (Matrix3d *)malloc(mid4.batch * sizeof(Matrix3d));
        for (int batch_idx = 0; batch_idx < mid4.batch; batch_idx++)
        {
            Matrix3d mid3 = mid4.matrix4d[batch_idx];
            Matrix3d output3d = conv_test_with_output(mid3, input_dim, output_channels, 
                                                    stride, kernel_size, mode, padding, false);
            output3d_arr[batch_idx] = output3d;
        }

        Matrix4d output4d = CreateMatrix4d(mid4.batch, output_channels, 
                                         output3d_arr[0].wid, output3d_arr[0].high);
        for (int batch_idx = 0; batch_idx < mid4.batch; batch_idx++)
        {
            output4d.matrix4d[batch_idx] = output3d_arr[batch_idx];
        }
        
        free(output3d_arr);
        return output4d;
    }

    Matrix4d backward(Matrix4d grad_output) override
    {
        Matrix4d input_grad = CreateMatrix4d(input_cache.batch, input_dim, 
                                           input_cache.wid, input_cache.high);
        
        // 初始化梯度为0
        for(int b = 0; b < input_cache.batch; b++) {
            for(int c = 0; c < input_dim; c++) {
                for(int h = 0; h < input_cache.wid; h++) {
                    for(int w = 0; w < input_cache.high; w++) {
                        input_grad.matrix4d[b].matrix3d[c].matrix[h][w] = 0.0f;
                    }
                }
            }
        }
        
        for(int batch_idx = 0; batch_idx < grad_output.batch; batch_idx++) {
            for(int out_c = 0; out_c < output_channels; out_c++) {
                for(int in_c = 0; in_c < input_dim; in_c++) {
                    // 旋转卷积核
                    Matrix rotated_kernel = rot180(kernel[in_c][out_c]);
                    
                    // 计算需要的padding大小
                    int pad_h = kernel_size - 1;
                    int pad_w = kernel_size - 1;
                    
                    // 对梯度进行padding
                    Matrix padded_grad = edge_padding(grad_output.matrix4d[batch_idx].matrix3d[out_c], 
                                                    grad_output.wid + 2*pad_h,
                                                    grad_output.high + 2*pad_w);
                    
                    // 进行完整卷积（stride=1）
                    for(int h = 0; h < input_cache.wid; h++) {
                        for(int w = 0; w < input_cache.high; w++) {
                            float sum = 0.0f;
                            for(int kh = 0; kh < kernel_size; kh++) {
                                for(int kw = 0; kw < kernel_size; kw++) {
                                    int h_p = h + kh;
                                    int w_p = w + kw;
                                    sum += padded_grad.matrix[h_p][w_p] * 
                                          rotated_kernel.matrix[kh][kw];
                                }
                            }
                            input_grad.matrix4d[batch_idx].matrix3d[in_c].matrix[h][w] += sum;
                        }
                    }
                    
                    free_mat(rotated_kernel);
                    free_mat(padded_grad);
                }
            }
        }
        
        return input_grad;
    }

    int parameter_counter()
    {
        int num_params = input_dim * output_channels * kernel_size * kernel_size;

        // if (bias)
        // {
        //     num_params += out_channels;
        // }

        return num_params;
    }

    // 添加设置卷积核的方法
    void set_kernel(float* kernel_values, int in_c, int out_c) {
        for(int i = 0; i < kernel_size; i++) {
            for(int j = 0; j < kernel_size; j++) {
                kernel[in_c][out_c].matrix[i][j] = kernel_values[i * kernel_size + j];
            }
        }
    }
};

class bn : public edge_layer
{
public:
    double beta = 0.1;
    double gamma = 0.1;
    bn(double beta, double gamma);
    Matrix3d forward(Matrix3d mid1)
    {
        Matrix3d output_bn = CreateMatrix3d(mid1.dep, mid1.wid, mid1.high);
        for (int i = 0; i < mid1.dep; i++)
        {
            double mean_bn = matrix_mean(mid1.matrix3d[i]);
            double var_bn = matrix_var(mid1.matrix3d[i]);
            // cout << "sqrt var:" << sqrt(var_bn) << endl;
            output_bn.matrix3d[i] = subtract_ele(mid1.matrix3d[i], mean_bn);
            // cout << "beta:" << beta << endl;
            output_bn.matrix3d[i] = matrix_division(output_bn.matrix3d[i], sqrt(var_bn));
            cout_mat(output_bn.matrix3d[i]);
            output_bn.matrix3d[i] = times_mat(beta, output_bn.matrix3d[i]);
            output_bn.matrix3d[i] = add_ele(output_bn.matrix3d[i], gamma);
            cout_mat(output_bn.matrix3d[i]);
        }
        return output_bn;
    }
};
bn::bn(double beta_bn, double gamma_bn)
{
    beta = beta_bn;
    gamma = gamma_bn;
}
class fc : public edge_layer
{
public:
    // int layer_call()
    // {
    //     std::cout << "call back from fc" << std::endl;
    //     return 100;
    // }
};
// enum LAYER_TYPE
// {
//     conv_2d,
//     batchnormal,
//     fullconnect
// };
class focus : public edge_layer
{
public:
    int in_channel = 32;
    int out_channel = 64;
    focus(int in_channel, int out_channel);
    Matrix3d forward(Matrix3d mid1)
    {
        Matrix4d template_mid1 = CreateMatrix4d(in_channel, mid1.dep, mid1.wid, mid1.high);
        edge_layer *conv2d_1 = new conv2d(template_mid1, 3, 12, 1, 9, 0, 5);
        Matrix3d output_focus;
        return output_focus;
    }
};
focus::focus(int in_channel, int out_channel)
{
    in_channel = in_channel;
    out_channel = out_channel;
}