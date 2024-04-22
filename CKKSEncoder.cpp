//
// Created by hanggu on 24-4-17.
//

#include "CKKSEncoder.h"
#include <Eigen/Dense>
#include <cmath>
#include <complex>
#include <iostream>
#include <random>


// CKKSEncoder::CKKSEncoder(int M) : M(M) {
//     double angle = 2 * M_PI / M;
//     xi = std::exp(std::complex<double>(0.0, angle));
// }

CKKSEncoder::CKKSEncoder(int M, double scale) : M(M), scale(scale) {
    double angle = 2 * M_PI / M;
    xi = std::exp(std::complex<double>(0.0, angle));
    create_sigma_R_basis();
}

std::vector<std::vector<std::complex<double>>> CKKSEncoder::vandermonde(std::complex<double> xi, int M) {
    int N = M / 2;
    std::vector<std::vector<std::complex<double>>> matrix;

    for (int i = 0; i < N; ++i) {
        std::complex<double> root = xi;
        root = std::pow(root, 2 * i + 1);
        std::vector<std::complex<double>> row;

        for (int j = 0; j < N; ++j) {
            row.push_back(std::pow(root, j));
        }

        matrix.push_back(row);
    }

    return matrix;
}

Polynomial<std::complex<double>> CKKSEncoder::sigma_inverse(std::vector<std::complex<double>>& b) {
    std::vector<std::vector<std::complex<double>>> A = vandermonde(xi, M);
    Eigen::MatrixXcd matrix_A(M/2, M/2);

    for (int i = 0; i < M/2; ++i) {
        for (int j = 0; j < M/2; ++j) {
            matrix_A(i, j) = A[i][j];
        }
    }

    Eigen::VectorXcd vector_b(M/2);
    for (int i = 0; i < M/2; ++i) {
        vector_b(i) = b[i];
    }

    Eigen::VectorXcd coeffs = matrix_A.fullPivLu().solve(vector_b);

    std::vector<std::complex<double>> polynomial_coeffs;
    for (int i = 0; i < coeffs.size(); ++i) {
        polynomial_coeffs.push_back(coeffs(i));
    }

    return Polynomial(polynomial_coeffs);
}

std::vector<std::complex<double>> CKKSEncoder::sigma(Polynomial<std::complex<double>>& p) {
    std::vector<std::complex<double>> outputs;
    int N = M / 2;

    for (int i = 0; i < N; ++i) {
        std::complex<double> root = xi;
        root = std::pow(root, 2 * i + 1);
        std::complex<double> output = p.eval(root);
        outputs.push_back(output);
    }

    return outputs;
}

std::vector<std::complex<double>> CKKSEncoder::pi(std::vector<std::complex<double>>& z) {
    int N = M / 4;
    std::vector<std::complex<double>> result(z.begin(), z.begin() + N);
    return result;
}

std::vector<std::complex<double>> CKKSEncoder::pi_inverse(std::vector<std::complex<double>>& z) {
    std::vector<std::complex<double>> z_conjugate(z.rbegin(), z.rend());
    for (auto& elem : z_conjugate) {
        elem = std::conj(elem);
    }
    std::vector<std::complex<double>> result(z.begin(), z.end());
    result.insert(result.end(), z_conjugate.begin(), z_conjugate.end());
    return result;
}

void CKKSEncoder::create_sigma_R_basis() {
    auto van = vandermonde(xi, M);
    //需要转置该矩阵
    sigma_R_basis.resize(M / 2, std::vector<std::complex<double>>(M / 2));
    for (int i = 0; i < M / 2; ++i) {
        for (int j = 0; j < M / 2; ++j) {
            sigma_R_basis[i][j] = van[j][i];
        }
    }
}

std::vector<std::vector<std::complex<double>>> CKKSEncoder::get_sigma_R_basis() {
    return sigma_R_basis;
}

std::vector<std::complex<double>> CKKSEncoder::matmul(const std::vector<std::vector<std::complex<double>>>& matrix, const std::vector<std::complex<double>>& vector) {
    std::vector<std::complex<double>> result(matrix.size());

    for (size_t i = 0; i < matrix.size(); ++i) {
        std::complex<double> sum(0.0, 0.0);
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            sum += matrix[i][j] * vector[j];
        }
        result[i] = sum;
    }

    return result;
}

std::vector<double> CKKSEncoder::compute_basis_coordinates(const std::vector<std::complex<double>>& z) {
    std::vector<double> output;
    output.reserve(sigma_R_basis.size());

    for (const auto& b : sigma_R_basis) {
        double dot_product = 0.0;
        double norm_b = 0.0;
        for (size_t i = 0; i < z.size(); ++i) {
            dot_product += std::real(std::conj(z[i]) * b[i]);
            norm_b += std::norm(b[i]);
        }
        output.push_back(dot_product / norm_b);
    }

    return output;
}

std::vector<double> CKKSEncoder::round_coordinates(const std::vector<double>& coordinates) {
    std::vector<double> result(coordinates.size());
    int i=0;
    for (; i < coordinates.size(); i++) {
        result[i] = coordinates[i] - std::floor(coordinates[i]);
    }
    return result;
}

//坐标随机取整。可能还需要在for循环中使用不同的随机数。现在的版本是每个坐标都使用相同的随机数。
std::vector<double> CKKSEncoder::coordinate_wise_random_rounding(const std::vector<double>& coordinates) {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    std::vector<double> r = round_coordinates(coordinates); // 获取坐标的舍入小数部分
    std::cout<<"小数部分:"<<std::endl;
    print_vector(r);
    std::vector<double> rounded_coordinates(r.size());

     for (size_t i = 0; i < r.size(); ++i) {
        double c = r[i];//当前数小数
        double f = (distribution(generator) > c) ? c : c - 1.0;//有1-c的概率（即distribution(generator) > c的概率）选择c，有c的概率选择c-1
        rounded_coordinates[i] = coordinates[i] - f;
    }

    // 将结果转换为整数
    std::transform(rounded_coordinates.begin(), rounded_coordinates.end(), rounded_coordinates.begin(), [](double val) {
        return static_cast<int>(val);
    });

    return rounded_coordinates;
}

std::vector<std::complex<double>> CKKSEncoder::sigma_R_discretization(const std::vector<std::complex<double>>& z) {
    std::vector<double> coordinates = compute_basis_coordinates(z);//计算z_i
    std::cout << "Coordinates: ";
    print_vector(coordinates);

    std::vector<double> rounded_coordinates = coordinate_wise_random_rounding(coordinates); //舍入取整
    std::cout << "Rounded Coordinates: ";
    print_vector(rounded_coordinates);

    std::vector<std::complex<double>> result;
    result.reserve(z.size());

    // 首先转置 sigma_R_basis
    std::vector<std::vector<std::complex<double>>> transposed_basis(sigma_R_basis[0].size(), std::vector<std::complex<double>>(sigma_R_basis.size()));
    for (size_t i = 0; i < sigma_R_basis.size(); ++i) {
        for (size_t j = 0; j < sigma_R_basis[i].size(); ++j) {
            transposed_basis[j][i] = sigma_R_basis[i][j];
        }
    }

    // 对于每个坐标向量，使用整个正交基矩阵与该向量进行乘积
    for (size_t i = 0; i < transposed_basis.size(); ++i) {
        std::complex<double> value = 0.0;
        for (size_t j = 0; j < transposed_basis[i].size(); ++j) {
            value += transposed_basis[i][j] * rounded_coordinates[j];
        }
        result.push_back(value);
    }

    return result;
}

Polynomial<double> CKKSEncoder::encode(std::vector<std::complex<double>> &z) {
    std::vector<std::complex<double>> pi_z = pi_inverse(z);
    std::cout << "pi_z: ";
    print_vector(pi_z);

    std::vector<std::complex<double>> scaled_pi_z(pi_z.size());
    for (size_t i = 0; i < pi_z.size(); ++i) {
        scaled_pi_z[i] = pi_z[i] * scale;
    }
    std::cout << "scaled_pi_z: ";
    print_vector(scaled_pi_z);

    std::vector<std::complex<double>> rounded_scaled_pi_zi = sigma_R_discretization(scaled_pi_z);//这个函数有问题
    std::cout << "rounded_scaled_pi_zi: ";
    print_vector(rounded_scaled_pi_zi);

    Polynomial p = sigma_inverse(rounded_scaled_pi_zi);

    // Due to numerical imprecision, we round the coefficients afterwards
//    std::vector<std::complex<double>> coef(p.degree() + 1);
    std::vector<double> coef(p.degree() + 1);
    for (int i = 0; i <= p.degree(); ++i) {
        coef[i] = std::round(std::real(p.getCoefficients()[i]));
    }
    std::cout<< "coef: ";
    print_vector(coef);

    return Polynomial(coef);
}

std::vector<std::complex<double>> CKKSEncoder::decode(const Polynomial<double>& p) {
    Polynomial<std::complex<double>>rescaled_p(p.degree());
    for (int i = 0; i <= rescaled_p.degree(); ++i) {
        rescaled_p.setCoefficientsByPos(i,std::complex<double>(p.getCoefficients()[i]/ scale,0));
    }
    std::cout << "rescaled_p: ";
    rescaled_p.print();

    std::vector<std::complex<double>> z = sigma(rescaled_p);
    std::vector<std::complex<double>> pi_z = pi(z);
    return pi_z;
}
