//
// Created by hanggu on 24-4-18.
//

#ifndef SIMPLE_CKKS_QPOLYNOMIAL_H
#define SIMPLE_CKKS_QPOLYNOMIAL_H
#include "Polynomial.h"

//R_q=Z_q[X]/(X^N + 1)的多项式
class QPolynomial : public Polynomial<double>{
private:
    // std::vector<int> coefficients;
    Polynomial<double>poly;//非环上的多项式
    
public:
    static double q;//静态成员变量（不属于对象而属于类），可以全局设置
    static int N;
    static Polynomial<double> poly_modulus;//多项式模

    QPolynomial()= default;
    explicit QPolynomial(std::vector<double>coef);
    explicit QPolynomial(Polynomial<double>poly);

    // 设置环系统。在生成QPolynomial对象之前，必须先设置环系统
    static void setRing(int qq,int NN);


    QPolynomial operator+(const QPolynomial& right) const;
    QPolynomial operator-(const QPolynomial& right) const;
    QPolynomial operator*(const QPolynomial& right) const;
    QPolynomial operator/(double scale) const;

    void print() const;


};

#endif //SIMPLE_CKKS_QPOLYNOMIAL_H
