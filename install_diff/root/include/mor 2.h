#ifndef MONADIC_OPERATION_RESULT
#define MONADIC_OPERATION_RESULT

struct MonadicOperationResult {
    double value;
    double grad;

    MonadicOperationResult(double value, double grad){
        this->value = value;
        this->grad = grad;
    }
};

#endif /* end of include guard: MONADIC_OPERATION_RESULT */
