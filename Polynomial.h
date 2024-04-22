//
// Created by hanggu on 24-4-16.
//

#ifndef SIMPLE_CKKS_POLYNOMIAL_H
#define SIMPLE_CKKS_POLYNOMIAL_H

#include <iostream>
#include <vector>
#include <complex>
#include <stdexcept>

//复数域上的多项式
template<typename T>
class Polynomial {
private:
    std::vector<T> coefficients;

public:
    Polynomial() = default;
    explicit Polynomial(const std::vector<T>& coeffs) : coefficients(coeffs) {}

   explicit Polynomial(int degree, T defaultCoefficient = T()) : coefficients(degree + 1, defaultCoefficient) {}

    // Polynomial(int degree, T defaultCoefficient) : coefficients(degree + 1, defaultCoefficient) {}

    int degree() const{
        return coefficients.size() - 1;
    };

    T eval(T x) const{
        T result(0);
        T x_power(1);

        for (int i = 0; i < coefficients.size(); ++i) {
            result += coefficients[i] * x_power;
            x_power *= x;
        }

        return result;        
    }

    Polynomial operator+(const Polynomial& other) const{
        int maxSize = std::max(coefficients.size(), other.coefficients.size());
        std::vector<T> resultCoeffs(maxSize, 0);

        for (int i = 0; i < maxSize; ++i) {
            if (i < coefficients.size()) {
                resultCoeffs[i] += coefficients[i];
            }
            if (i < other.coefficients.size()) {
                resultCoeffs[i] += other.coefficients[i];
            }
        }
        return Polynomial(resultCoeffs);
    }


    Polynomial operator*(const Polynomial& other) const{
        int resultDegree = degree() + other.degree();
        std::vector<T> resultCoeffs(resultDegree + 1, 0);

        for (int i = 0; i <= degree(); ++i) {
            for (int j = 0; j <= other.degree(); ++j) {
                resultCoeffs[i + j] += coefficients[i] * other.coefficients[j];
            }
        }

        return Polynomial(resultCoeffs);        
    }

    //友元函数，使得左操作数为数字，右操作数为多项式
    friend Polynomial operator*(const T& scalar, const Polynomial<T>& poly) {
        Polynomial<T> result(poly);
        for (T& coef : result.coefficients) {
            coef *= scalar;
        }
        return result;
    }

    //多项式环上的模运算（直接假定模为分圆多项式 x^N+1）
    Polynomial operator%(const Polynomial& divisor) const{
        Polynomial dividend = *this;
        Polynomial remainder;

        if(dividend.degree()<divisor.degree()) return dividend;

        std::vector<T> resultCoeffs(divisor.degree());//多项式环上的模运算，结果的次数最多是除数的次数-1

        for(int i=0;i<dividend.coefficients.size();i++){
            //i为被除数当前项的次数
            if(i<divisor.degree()) resultCoeffs[i] += dividend.coefficients[i];
            else{
                int pos = i % divisor.degree();
                if((i / divisor.degree()) % 2) resultCoeffs[pos] -= dividend.coefficients[i];
                else resultCoeffs[pos] += dividend.coefficients[i];
            }
        }

        //将resultCoeffs尾部的0去
        while(!resultCoeffs.empty() && resultCoeffs[resultCoeffs.size()-1]==T()) {
            resultCoeffs.pop_back();
        }

        remainder = Polynomial(resultCoeffs);
        return remainder;        
    }


    Polynomial operator-(const Polynomial& other) const{
        int maxSize = std::max(coefficients.size(), other.coefficients.size());
        std::vector<T> resultCoeffs(maxSize, 0);

        for (int i = 0; i < maxSize; ++i) {
            if (i < coefficients.size()) {
                resultCoeffs[i] += coefficients[i];
            }
            if (i < other.coefficients.size()) {
                resultCoeffs[i] -= other.coefficients[i];
            }
        }

        return Polynomial(resultCoeffs);        
    }

    //求相反多项式
    Polynomial operator-() const {
        std::vector<T> resultCoeffs(coefficients.size(), 0);

        for (int i = 0; i < coefficients.size(); ++i) {
            resultCoeffs[i] = -coefficients[i];
        }

        return Polynomial(resultCoeffs);
    }

    Polynomial operator/(double scale) const{
        if (scale == 0) {
            throw std::invalid_argument("Division by zero");
        }

        std::vector<double> resultCoeffs(coefficients.size(), 0);

        for (int i = 0; i < coefficients.size(); ++i) {
            resultCoeffs[i] = coefficients[i] / scale;
        }

        return Polynomial(resultCoeffs);
    }
    

    std::vector<T> getCoefficients() const{
        return coefficients;
    }

    void setCoefficientsByPos(int pos, const T& value){
        if (pos < 0 || pos >= coefficients.size()) {
            throw std::out_of_range("Index out of range");
        }
        coefficients[pos] = value;        
    }

    void print() const{
        bool isFirstTerm = true;

        for (int i = coefficients.size() - 1; i >= 0; --i) {
            if (coefficients[i] != T(0)) {
                if (!isFirstTerm) {
                    std::cout << " + ";
                } else {
                    isFirstTerm = false;
                }
                std::cout << coefficients[i];
                if (i != 0) {
                    std::cout << "x^" << i;
                }
            }
        }
        std::cout << std::endl;        
    }
};

#endif //SIMPLE_CKKS_POLYNOMIAL_H
