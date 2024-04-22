#ifndef SIMPLE_CKKS_CKKSENCRYPTOR_H
#define SIMPLE_CKKS_CKKSENCRYPTOR_H


#include "Context.h"
#include "Polynomial.h"
#include "PolynomialSampler.h"
class CKKSEncryptor {
private:
    std::pair<Polynomial<double>, Polynomial<double>> pk;
    Context context;
    PolyNomialSampler sampler;

public:
    CKKSEncryptor(Context context, std::pair<Polynomial<double>, Polynomial<double>> pk);
    std::pair<Polynomial<double>, Polynomial<double>> encrypt(const Polynomial<double>& ptx);
};

#endif //SIMPLE_CKKS_CKKSENCRYPTOR_H