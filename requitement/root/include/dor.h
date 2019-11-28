#ifndef DYADIC_OPERATION_RESULT
#define DYADIC_OPERATION_RESULT

struct DyadicOperationResult {
    double value;
    double left_grad;
    double right_grad;

    DyadicOperationResult(double value, double left_grad, double right_grad){
        this->value = value;
        this->left_grad = left_grad;
        this->right_grad = right_grad;
    }
};

#endif /* end of include guard: DYADIC_OPERATION_RESULT */
