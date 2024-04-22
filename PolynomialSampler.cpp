#include "PolynomialSampler.h"
#include <algorithm>
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

Polynomial<double> PolyNomialSampler::DG(){
    int64_t q=context.get_q();
    int N=context.get_N();
    int sigma=context.get_sigma();
    // Declare random_engine variable and seed it with a non-deterministic value
    std::random_device rd;
    std::default_random_engine random_engine(rd());
    // 生成正态分布的随机系数
    std::vector<double> coeffs(N);
    for (int i = 0; i < N; ++i) {
        coeffs[i] = std::normal_distribution<double>(0.0, sigma)(random_engine);
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
    int N=context.get_N();
    int sigma=context.get_sigma();
    // Declare random_engine variable and seed it with a non-deterministic value
    std::random_device rd;
    std::default_random_engine random_engine(rd());
    // 从 [0, q) 范围内随机选择 N 个整数作为系数
    std::vector<double> coeffs(N);
    for (int i = 0; i < N; ++i) {
        coeffs[i] = std::uniform_int_distribution<int>(0, q - 1)(random_engine);
    }

    // 对系数进行取模运算
    std::vector<double> modded_coeffs(N);
    modded_coeffs = mod_q(coeffs, q);

    // 生成多项式
    Polynomial<double> p(modded_coeffs);

    return p;

}

//ZO(rou)表示从{1,-1,0}均匀采样N个系数并保证1出现的概率为rou/2，-1出现的概率为rou/2，0出现的概率为1-rou
Polynomial<double> PolyNomialSampler::ZO(){
    int N=context.get_N();
    int sigma=context.get_sigma();
    double p=context.get_p();
        std::vector<double> coeffs(N);

    for (int i = 0; i < N; ++i) {
        double rand_val = (double)std::rand() / RAND_MAX;
        if (rand_val < 1 - p) {
            coeffs[i] = 0;
        } else if (rand_val < 1 - p + p / 2) {
            coeffs[i] = 1;
        } else {
            coeffs[i] = -1;
        }
    }

    return Polynomial(coeffs);
}

Polynomial<double> PolyNomialSampler::HWT(){
    int N=context.get_N();
    int h=context.get_h();
    std::vector<double> coeffs(N);

    // 设置所有系数为 -1 或 1
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 1);
    for (int i = 0; i < N; ++i) {
        coeffs[i] = distribution(gen) == 0 ? 1 : -1;
    }

    // 随机选择并将一部分系数设为 0
    std::vector<int> slots(N);
    for (int i = 0; i < N; ++i) {
        slots[i] = i;
    }
    std::shuffle(slots.begin(), slots.end(), std::mt19937(std::random_device()()));
    for (int i = 0; i < N - h; ++i) {
        coeffs[slots[i]] = 0;
    }

    return Polynomial(coeffs);
}

