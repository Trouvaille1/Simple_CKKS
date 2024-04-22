#ifndef SIMPLE_CKKS_CKKSDECRYPTOR_H
#define SIMPLE_CKKS_CKKSDECRYPTOR_H

#include "Context.h"
class CKKSDecryptor {
private:
    Polynomial<double> sk;
    Context context;

public:
    CKKSDecryptor(Context context, Polynomial<double> sk);
    Polynomial<double> decrypt(std::pair<Polynomial<double>, Polynomial<double>>& ctx);
};


#endif //SIMPLE_CKKS_CKKSDECRYPTOR_H