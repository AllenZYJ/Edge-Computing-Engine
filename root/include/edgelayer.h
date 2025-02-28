class edge_layer
{
public:
    virtual ~edge_layer() {}
    virtual Matrix4d forward(Matrix4d mid1) = 0;
    virtual int parameter_counter() = 0;
};

class conv2d : public edge_layer
{
public:
    conv2d(Matrix4d mid_4, int in_channel, int out_channle, int _stride, int ksize, int _mode, int _padding);
    int arg1;
    Matrix4d mid4;
    int input_dim;
    int output_channels;
    int stride;
    int kernel_size;
    int mode;
    int padding;

    Matrix4d forward(Matrix4d mid4)
    {
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
            Matrix3d output3d = conv_test_with_output(mid3, input_dim, output_channels, stride, kernel_size, mode, false);
            output3d_arr[batch_idx] = output3d;
        }

        Matrix4d output4d = CreateMatrix4d(mid4.batch, output_channels, output3d_arr[0].wid, output3d_arr[0].high);
        for (int batch_idx = 0; batch_idx < mid4.batch; batch_idx++)
        {
            output4d.matrix4d[batch_idx] = output3d_arr[batch_idx];
        }
        return output4d;
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
};

conv2d::conv2d(Matrix4d mid_1, int in_channel, int out_channle, int _stride, int ksize, int _mode, int _padding)
{
    mid4 = mid_1;
    input_dim = in_channel;
    output_channels = out_channle;
    stride = _stride;
    kernel_size = ksize;
    mode = _mode;
    padding = _padding;
}
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