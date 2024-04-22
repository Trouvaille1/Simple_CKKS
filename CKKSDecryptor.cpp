#include "CKKSDecryptor.h"

#include <utility>

CKKSDecryptor::CKKSDecryptor(Context context, Polynomial<double> sk) : context(std::move(context)), sk(std::move(sk)) {}

Polynomial<double> CKKSDecryptor::decrypt(std::pair<Polynomial<double>, Polynomial<double>> &ctx) {
    auto mm=ctx.second*sk;
    auto ptx=ctx.first+mm;
    return ptx;
}