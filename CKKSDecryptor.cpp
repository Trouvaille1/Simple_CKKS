#include "CKKSDecryptor.h"

#include <utility>

CKKSDecryptor::CKKSDecryptor(Context context, Polynomial<double> sk) : context(std::move(context)), sk(std::move(sk)) {}

Polynomial<double> CKKSDecryptor::decrypt(std::pair<Polynomial<double>, Polynomial<double>> &ctx) {
    auto ptx=ctx.first + sk * ctx.second;
    return ptx;
}