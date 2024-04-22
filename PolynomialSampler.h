#ifndef SIMPLE_CKKS_POLYNOMIALSAMPLER_H
#define SIMPLE_CKKS_POLYNOMIALSAMPLER_H

#include "Context.h"
#include "Polynomial.h"
#include "QPolynomial.h"


class PolyNomialSampler {
private:
    Context &context;
public:
    explicit PolyNomialSampler(Context &context);
    std::vector<double> mod_q(const std::vector<double>& coeffs, int64_t q);//将给定系数向量进行取模运算，其中超过 q/2 的系数被映射到 (-q/2, 0] 的范围内
    Polynomial<double> DG();//DG(sigma^2)表示从离散高斯分布中采样N个系数从而获得多项式e属于Rq(sigma=3)
    Polynomial<double> UniformPolynomial(int64_t q=QPolynomial::q);//UniformPolynomial
    Polynomial<double> ZO();//ZO(rou)表示从{1,-1,0}均匀采样N个系数并保证1出现的概率为rou/2，-1出现的概率为rou/2，0出现的概率为1-rou(rou=0.5)
    Polynomial<double> HWT();//HWT(h)表示从{1,-1,0}均匀采样N个系数并保证非零系数的个数恰为h(h=64)，这些系数组成了多项式s属于Rq
};

#endif //SIMPLE_CKKS_POLYNOMIALSAMPLER_H