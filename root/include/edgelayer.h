class edge_layer
{
public:
    virtual ~edge_layer() {}
    virtual void layer_call() = 0;
};

class conv2d: public edge_layer
{
public:
    conv2d(Matrix mid_1,int in_channel,int out_channle,int _stride,int ksize,int _mode,int _padding);
    int arg1;
    Matrix mid1;
    int input_dim;
    int output_channels;
    int stride;
    int kernel_size;
    int mode;
    int padding;
    void layer_call() 
    {
    cout_mat(mid1);
    Matrix mid_rgb[input_dim];
    for(int rgb_idx = 0;rgb_idx<input_dim;rgb_idx++)
    {
        mid_rgb[rgb_idx] = ones(mid1.row,mid1.col);

    }
    Matrix filters[output_channels][input_dim];
    for(int channel_index = 0;channel_index<input_dim;channel_index++)
    {
        for(int filter_index = 0;filter_index<output_channels;filter_index++)
            {
                Matrix kernel = ones(kernel_size,kernel_size);
                filters[channel_index][filter_index] = kernel;      
            }
    }
    if(mode == 0)
    {
        cout<<"input_img:"<<endl;
        for(int i =0;i<input_dim;i++)
        {
            cout<<"---------rgb: "<<i<<"---------"<<endl;
            cout_mat(mid_rgb[i]);
        }
        Matrix conv_result = CreateMatrix(((mid1.row-kernel_size)/stride)+1,((mid1.col-kernel_size)/stride)+1);
        Matrix kernel = ones(kernel_size,kernel_size);
        cout<<"--------- kernels: 3x3--------"<<endl;
        cout_mat(kernel);
        cout<<"--------- output: ---------"<<endl;  
        Matrix feature_maps[output_channels];
        for(int filter_idx = 0;filter_idx<output_channels;filter_idx++)
        {   
            Matrix sum_rgb = CreateMatrix(((mid1.row-kernel_size)/stride)+1,((mid1.col-kernel_size)/stride)+1);
        for(int channel_idx=0;channel_idx<input_dim;channel_idx++)
        {

            sum_rgb = add(sum_rgb,conv_element(mid_rgb[channel_idx],filters[channel_idx][filter_idx],kernel_size,stride),0);
            cout<<"sum_rgb"<<"filters_index: "<<filter_idx<<" "<<endl;
            cout_mat(sum_rgb);
        }
        feature_maps[filter_idx]=sum_rgb;
    }
    for(int i = 0;i < output_channels;i++)
    {
        cout<<"==========filter: "<<i<<"========="<<endl;
        cout_mat(feature_maps[i]);
    }
    }
}
};
conv2d::conv2d(Matrix mid_1,int in_channel,int out_channle,int _stride,int ksize,int _mode,int _padding){
    mid1 = mid_1;
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
    int arg1bn;
    int arg2bn;
    bn(int bn_input_dime,int bn_output_dim);    
    void layer_call()
    {
        std::cout << "call back from bn" << arg1bn <<"="<<arg2bn<<std::endl;
    }
};
bn::bn(int bn_input_dime,int bn_output_dim){
    arg2bn = bn_input_dime;
    arg1bn = bn_output_dim;
}
class fc : public edge_layer
{
public:
    void layer_call()
    {
        std::cout << "call back from fc" << std::endl;
    }
};
// enum LAYER_TYPE
// {
//     conv_2d,
//     batchnormal,
//     fullconnect
// };

// class LayerFactory
// {
// public:
//     edge_layer *CreateLayer(LAYER_TYPE type,int indime = 3)
//     {
//         switch (type)
//         {
//         case conv_2d:
//             return new conv2d(indime);
//             break;
//         default:
//             return NULL;
//             break;
//         }
//     }
// };
