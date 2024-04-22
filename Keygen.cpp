#include "Keygen.h"

Keygen::Keygen(Context context) : context(context),sampler(context) {}

Polynomial<double> Keygen::generate_secret_key() {
    return sampler.HWT();
}

std::pair<Polynomial<double>, Polynomial<double>> Keygen::generate_public_key(const Polynomial<double>& s) {
    Polynomial<double> a = sampler.UniformPolynomial();
    Polynomial<double> e = sampler.DG();

    Polynomial<double> b = -(a * s) + e;

    // std::pair<Polynomial<int>, Polynomial<int>> pk(b, a);
    std::pair<Polynomial<double>, Polynomial<double>> pk = std::make_pair(b, a);
    return pk;
}

std::pair<Polynomial<double>, Polynomial<double>> Keygen::generate_relin_key(const Polynomial<double>& s) {
    // Polynomial<int> a = context.uniform.sample_manually(context.q * context.special_prime);
    Polynomial<double> a= sampler.UniformPolynomial(context.get_q() * context.get_special_prime());
    Polynomial<double> e = sampler.DG();

    Polynomial<double> b = -(a * s) + e + context.get_special_prime() * s * s;
    std::pair<Polynomial<double>, Polynomial<double>> relin_key = std::make_pair(b, a);
    return relin_key;
}
