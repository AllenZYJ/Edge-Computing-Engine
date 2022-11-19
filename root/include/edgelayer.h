class edge_layer
{
public:
    virtual ~edge_layer() {}
    virtual void layer_call() = 0;
};

class conv2d: public edge_layer
{
public:
    void layer_call()
    {
        cout_mat(ones(3,3));
        std::cout << "call back from conv2d layer" << std::endl;
    }
    conv2d(int input_dime);
};
conv2d::conv2d(int input_dime){
    std::cout<<"input_dime :"<<input_dime<<std::endl;
}
class bn : public edge_layer
{
public:
    void layer_call()
    {
        std::cout << "call back from bn" << std::endl;
    }
};

class fc : public edge_layer
{
public:
    void layer_call()
    {
        std::cout << "call back from fc" << std::endl;
    }
};
enum LAYER_TYPE
{
    conv_2d,
    batchnormal,
    fullconnect
};

class LayerFactory
{
public:
    edge_layer *CreateLayer(LAYER_TYPE type,int indime = 3)
    {
        switch (type)
        {
        case conv_2d:
            return new conv2d(indime);
            break;
        case batchnormal:
            return new bn();
            break;
        case fullconnect:
            return new fc();
            break;
        default:
            return NULL;
            break;
        }
    }
};
