#ifndef GRADIENT
#define GRADIENT

#include "dual.h"
#include <vector>

namespace dual{
    namespace{
        typedef std::vector<Dual> Vector;
        typedef std::vector<Vector> Matrix;
    }

    void resetDerivatives(Vector& input);
    Vector gradient(Dual (*f)(Vector&), Vector& input);
    Matrix jacobian(Vector (*f)(Vector&), Vector& input);
}

#endif
