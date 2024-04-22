#include "PolynomialSampler.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>
#include "CKKSEncoder.h"
#include "Context.h"
#include "Polynomial.h"

PolyNomialSampler::PolyNomialSampler(Context &context):context(context){}

std::vector<double> PolyNomialSampler::mod_q(const std::vector<double>& coeffs, int64_t q) {
    std::vector<double> r(coeffs.size());

    for (size_t i = 0; i < coeffs.size(); ++i) {
        r[i] = static_cast<int>(coeffs[i]) % q;
        if (r[i] > q / 2) {
            r[i] -= q;
        }
    }

    return r;
}

std::vector<double> PolyNomialSampler::mod_q(const std::vector<int64_t>& coeffs, int64_t q) {
    std::vector<double> r(coeffs.size());

    for (size_t i = 0; i < coeffs.size(); ++i) {
        r[i] = static_cast<int64_t>(coeffs[i]) % q;
        if (r[i] > q / 2) {
            r[i] -= q;
        }
    }

    return r;
}

Polynomial<double> PolyNomialSampler::DG(){
    int64_t q=context.get_q();
    int N=context.get_N();
    int sigma=context.get_sigma();
    // Declare random_engine variable and seed it with a non-deterministic value
    std::random_device rd;
    // std::default_random_engine random_engine(rd());
    std::mt19937 gen(rd());
    std::normal_distribution<> dis(0, sigma); // mean=0, stddev=sigma
    // 生成正态分布的随机系数
    std::vector<double> coeffs(N);
    for (int i = 0; i < N; ++i) {
        coeffs[i] = round(dis(gen));
    }

    CKKSEncoder encoder;//为了使用coordinate_wise_random_rounding函数
    // 对系数进行舍入取整
    std::vector<double> rounded_coeffs = encoder.coordinate_wise_random_rounding(coeffs);

    // 对系数进行取模运算
    std::vector<double> modded_coeffs(N);
    modded_coeffs = mod_q(rounded_coeffs, q);

    // 生成多项式
    Polynomial<double> p(modded_coeffs);

    return p;

}

Polynomial<double> PolyNomialSampler::UniformPolynomial(int64_t q){
    if(q==-1)q=context.get_q();//不能直接将类成员变量作为成员函数的参数默认值.所以将q默认值为-1，然后在函数内部赋值
    std::cout<<"UniformPolynomial中的q："<<q<<std::endl;
    int N=context.get_N();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int64_t> dis(0, q - 1);

    // 从 [0, q) 范围内随机选择 N 个整数作为系数
    std::vector<int64_t> coeffs(N);
    for (int i = 0; i < N; ++i) {
        coeffs[i] = dis(gen);
    }

    std::cout<<"mod_q之前的coeffs："<<std::endl;
    for(auto elem:coeffs){
        std::cout<<elem<<" ";
    }
    std::cout<<std::endl;

    // 对系数进行取模运算
    std::vector<double> modded_coeffs(N);
    modded_coeffs = mod_q(coeffs, q);
    std::cout<<"mod_q之后的modded_coeffs："<<std::endl;
    for(auto elem:modded_coeffs){
        std::cout<<elem<<" ";
    }
    std::cout<<std::endl;
    

    // 生成多项式
    Polynomial<double> p(modded_coeffs);

    return p;

}

//ZO(rou)表示从{1,-1,0}均匀采样N个系数并保证1出现的概率为rou/2，-1出现的概率为rou/2，0出现的概率为1-rou
Polynomial<double> PolyNomialSampler::ZO(){
    int N=context.get_N();
    double p=context.get_p();
    std::vector<double> coeffs(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> weights = {p/2.0, 1 - p, p / 2.0};//0，1,2的概率
    std::discrete_distribution<int> dis(weights.begin(), weights.end());

    // Generate random coefficients based on probabilities
    for (int i = 0; i < N; ++i) {
        coeffs[i] = dis(gen) - 1; // 因为weights表示的是0,1,2的概率，所以这里减一变成-1,0,1的概率
    }

    return Polynomial(coeffs);
}

Polynomial<double> PolyNomialSampler::HWT(){
    int N=context.get_N();
    int h=context.get_h();
    std::vector<double> coeffs(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1, 1);

    // Fill coeffs with random values {-1, 1}
    for (int i = 0; i < N; ++i) {
        coeffs[i] = dis(gen);
    }

    // Randomly select slots to be set to 0
    std::vector<int> slots(N);
    std::iota(slots.begin(), slots.end(), 0);
    std::shuffle(slots.begin(), slots.end(), gen);
    for (int i = 0; i < N - h; ++i) {
        coeffs[slots[i]] = 0;
    }

    Polynomial p(coeffs);
    return p;
}

