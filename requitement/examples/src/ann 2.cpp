#include <iostream>
#include <ctime>

#include "../../root/include/vectmath.h"
#include "../../root/include/node.h"

typedef std::vector<Node> Vector;
typedef std::vector<Vector> Matrix;

Node random_number(){
    return rand()/(double)RAND_MAX;
}

Node tan_h(Node& x){
    return (1-exp(-2*x))/(1+exp(-2*x));
}

Node mean_square_error(Vector& y_true, Vector& y_pred){
    Node loss;
    for(size_t i=0 ; i<y_true.size() ; i++){
        loss += pow(y_true[i]-y_pred[i], 2);
    }
    return loss;
}

struct Layer {
    Matrix weights;
    Matrix bias;
    Node (*activation)(Node&);
    int input_shape;
    int output_shape;

    Layer(int input, int output, Node (*activation)(Node&)){
        this->activation = activation;
        this->input_shape = input;
        this->output_shape = output;
        weights.resize(input, Vector(output));
        bias.resize(1, Vector(output));
        random_number >> weights;
        random_number >> bias;
    }

    Matrix forward(Matrix& previous){
        Matrix output = dot(previous, weights) + bias;
        output = activation >> output;
        return output;
    }

    void backward(Node& loss, const float& learning_rate){
        weights -= learning_rate*loss.gradient(weights);
        bias -= learning_rate*loss.gradient(bias);
    }
};

struct Network {
    std::vector<Layer> layers;
    int input_shape;
    Graph* graph;

    Network(){
        graph = Graph::getInstance();
    }

    void input_layer(int input_shape){
        this->input_shape = input_shape;
    }

    void add(int output_shape, Node (*activation)(Node&)){
        int input = layers.empty()?input_shape:layers.back().output_shape;
        layers.push_back(Layer(input, output_shape, activation));
    }

    Matrix run(Matrix& input){
        Matrix output(input.size());
        for(size_t j=0 ; j<input.size() ; j++){
            Matrix out = {input[j]};
            for(auto& lay : layers){
                out = lay.forward(out);
            }
            output[j] = out[0];
        }
        return output;
    }

    void fit(Matrix& input, Matrix& output, Node (*loss_function)(Vector&, Vector&), int epochs, float learning_rate){
        int p=0;
        for(size_t i=0 ; i<epochs ; i++){
            std::cout << "\r" << i+1 << "/" << epochs;
            for(size_t j=0 ; j<input.size() ; j++){
                // compute input
                Matrix out = {input[j]};
                for(auto& lay : layers){
                    out = lay.forward(out);
                }

                // compute loss
                Node loss = loss_function(output[j], out[0]);

                // update parameters
                for(auto& lay : layers){
                    lay.backward(loss, learning_rate);
                }

                graph->new_recording();
            }
        }
        std::cout << std::endl;
    }
};

int main(int argc, char const *argv[]) {
    srand(time(NULL));

    Matrix input = {{0,0},{0,1},{1,0},{1,1}};
    Matrix output = {{0},{1},{1},{0}};

    Network network;
    network.input_layer(2);
    network.add(3, tan_h);
    network.add(1, tan_h);
    network.fit(input, output, mean_square_error, 500, 0.1);

    Matrix pred = network.run(input);
    std::cout << pred << std::endl;
    return 0;
}
