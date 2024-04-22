//
// Created by hanggu on 24-4-18.
//

#ifndef SIMPLE_CKKS_CONTEXT_H
#define SIMPLE_CKKS_CONTEXT_H

#include "QPolynomial.h"

class Context {
private:
    int N;
    int M;
    int64_t q;//模数很大，要用long long存储
    int special_prime;

    //采样函数的参数
    int h;
    int sigma;
    double p;

    QPolynomial QPolynomial;

public:
    Context(int N,std::vector<int> moduli);
    void setup_parameters();//设置全局变量h、p、sigma供PolynomialSampler使用
    [[nodiscard]] int get_N() const{return N;}
    [[nodiscard]] int get_M() const{return M;}
    [[nodiscard]] int64_t get_q() const{return q;}
    [[nodiscard]] int get_h() const{return h;}
    [[nodiscard]] int get_sigma() const{return sigma;}
    [[nodiscard]] double get_p() const{return p;}
    [[nodiscard]] int get_special_prime() const{return special_prime;}

};


#endif //SIMPLE_CKKS_CONTEXT_H
