#ifndef SIMPLE_CKKS_KEYGEN_H
#define SIMPLE_CKKS_KEYGEN_H


#include "Context.h"
#include "PolynomialSampler.h"
class Keygen {
private:
    Context context;
    PolyNomialSampler sampler;
public:
    explicit Keygen(Context context);
    Polynomial<double> generate_secret_key();
    std::pair<Polynomial<double>, Polynomial<double>> generate_public_key(const Polynomial<double>& s);
    std::pair<Polynomial<double>, Polynomial<double>> generate_relin_key(const Polynomial<double>& s);

};

#endif //SIMPLE_CKKS_KEYGEN_H