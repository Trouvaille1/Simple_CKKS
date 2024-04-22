//
// Created by hanggu on 24-4-18.
//

#include "QPolynomial.h"
#include <cassert>
#include <utility>

double QPolynomial::q;
int QPolynomial::N;
Polynomial<double> QPolynomial::poly_modulus;


QPolynomial::QPolynomial(std::vector<double> coef) {
    poly = Polynomial<double>(coef);
    poly = poly % QPolynomial::poly_modulus;
    for (int i = 0; i < poly.getCoefficients().size(); i++) {
        poly.setCoefficientsByPos(i, static_cast<double>(static_cast<int>(poly.getCoefficients()[i]) % static_cast<int>(q)));
    }
}

void QPolynomial::setRing(int qq,int NN){
    QPolynomial::q=qq;
    QPolynomial::N=NN;

    std::vector<double>coef(QPolynomial::N+1,0);
    coef[0]=1;
    coef[QPolynomial::N]=1;
    QPolynomial::poly_modulus = Polynomial<double>(coef);
}

QPolynomial::QPolynomial(Polynomial<double>poly):poly(std::move(poly)){}

QPolynomial QPolynomial::operator+(const QPolynomial& right) const{
    return QPolynomial((poly + right.poly)%QPolynomial::poly_modulus);
}

QPolynomial QPolynomial::operator-(const QPolynomial& right) const{
    return QPolynomial((poly - right.poly)%QPolynomial::poly_modulus);
}

QPolynomial QPolynomial::operator*(const QPolynomial& right) const{
    return QPolynomial((poly * right.poly)%QPolynomial::poly_modulus);
}

QPolynomial QPolynomial::operator/(double scale) const{
    return QPolynomial(poly/scale);
}

void QPolynomial::print() const{
    poly.print();
}

