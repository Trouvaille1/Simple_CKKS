//
// Created by hanggu on 24-4-17.
//

#ifndef SIMPLE_CKKS_CKKSENCODER_H
#define SIMPLE_CKKS_CKKSENCODER_H

#include "Polynomial.h"
#include "helper.h"
#include <complex>
#include <vector>


class CKKSEncoder {
private:
    std::complex<double> xi;
    int M;
    double scale;
    std::vector<std::vector<std::complex<double>>> sigma_R_basis;
    int slot_count; 

    static std::vector<std::vector<std::complex<double>>> vandermonde(std::complex<double> xi, int M);

public:
    CKKSEncoder() = default;
    // explicit CKKSEncoder(int M);
    CKKSEncoder(int M, double scale);
    [[nodiscard]] int get_slot_count() const{return slot_count;}

    Polynomial<std::complex<double>> sigma_inverse(std::vector<std::complex<double>>& b);
    std::vector<std::complex<double>> sigma(Polynomial<std::complex<double>>& p);

    std::vector<std::complex<double>> pi(std::vector<std::complex<double>>& z);//从H=C^N缩减到C^{N/2}
    std::vector<std::complex<double>> pi_inverse(std::vector<std::complex<double>>& z);//从C^{N/2}扩展到H=C^N

    void create_sigma_R_basis();
    std::vector<std::vector<std::complex<double>>> get_sigma_R_basis();

    std::vector<std::complex<double>> matmul(const std::vector<std::vector<std::complex<double>>>& matrix, const std::vector<std::complex<double>>& vector);
    std::vector<double> compute_basis_coordinates(const std::vector<std::complex<double>>& z);//计算向量相对于正交格基的坐标.这里的正交格基是sigma_R_basis，z_i=<z,b_i>/||b_i^2||
    std::vector<double> round_coordinates(const std::vector<double>& coordinates);//计算每个坐标的小数部分
    std::vector<double> coordinate_wise_random_rounding(const std::vector<double>& coordinates);
    std::vector<std::complex<double>> sigma_R_discretization(const std::vector<std::complex<double>>& z);//使用坐标随机取整将H=C^N映射到sigma_R上

    Polynomial<double> encode(std::vector<std::complex<double>> &z);//实际上返回值应该是int类型的，但是这里为了方便测试，返回double类型
    std::vector<std::complex<double>> decode(const Polynomial<double>& p);
};

#endif //SIMPLE_CKKS_CKKSENCODER_H
