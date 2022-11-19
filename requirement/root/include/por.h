#ifndef POLYADIC_OPERATION_RESULT
#define POLYADIC_OPERATION_RESULT

#include <vector>

struct PolyadicOperationResult {
    double value;
    std::vector<double> gradients;

    PolyadicOperationResult(double value, const std::vector<double>& gradients){
        this->value = value;
        this->gradients = gradients;
    }
};

#endif /* end of include guard: POLYADIC_OPERATION_RESULT */
