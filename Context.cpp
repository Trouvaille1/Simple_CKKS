//
// Created by hanggu on 24-4-18.
//

#include "Context.h"
#include <iostream>
#include <numeric>

Context::Context(int N, std::vector<int> moduli):
N(N),
M(N*2),
q(std::accumulate(moduli.begin(), moduli.end()-1, 1, std::multiplies<int>())),
special_prime(moduli.back())
{
    QPolynomial::setRing(q,N);
    setup_parameters(); 
}

void Context::setup_parameters() {
    h=64;
    sigma=3;
    p=0.5;
}
