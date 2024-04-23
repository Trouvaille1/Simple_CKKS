#include "Keygen.h"

Keygen::Keygen(Context context) : context(context),sampler(context) {}

Polynomial<double> Keygen::generate_secret_key() {
    return sampler.HWT();
}

std::pair<Polynomial<double>, Polynomial<double>> Keygen::generate_public_key(const Polynomial<double>& s) {
    Polynomial<double> a = sampler.UniformPolynomial();
    Polynomial<double> e = sampler.DG();//高斯分布多项式

    Polynomial<double> b = -(a * s) + e;

    // std::pair<Polynomial<int>, Polynomial<int>> pk(b, a);
    std::pair<Polynomial<double>, Polynomial<double>> pk = std::make_pair(b, a);
    return pk;
}

std::pair<Polynomial<double>, Polynomial<double>> Keygen::generate_relin_key(const Polynomial<double>& s) {
    Polynomial<double> a= sampler.UniformPolynomial(context.get_q() * context.get_special_prime());//a从Rp*q中采样
    Polynomial<double> e = sampler.DG();

    Polynomial<double> b = -(a * s) + e + pow(2,12) * s * s;
    // std::cout<<"get_special_prime="<<context.get_special_prime()<<std::endl;
    //(b,a)要Mod p*qL
    b=b%(pow(2,12)*context.get_special_prime());
    a=a%(pow(2,12) * context.get_special_prime());
    std::cout<<"p*q="<<context.get_q() * context.get_special_prime()<<std::endl;

    std::pair<Polynomial<double>, Polynomial<double>> relin_key = std::make_pair(b, a);
    std::cout<<"生成的重线性化密钥relin_key[0]："<<std::endl;
    b.print();
    std::cout<<"生成的重线性化密钥relin_key[1]："<<std::endl;
    a.print();
    return relin_key;
}
