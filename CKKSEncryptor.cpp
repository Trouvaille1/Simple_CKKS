#include "CKKSEncryptor.h"
#include "Context.h"
#include "PolynomialSampler.h"
#include <utility>


CKKSEncryptor::CKKSEncryptor(Context context, std::pair<Polynomial<double>, Polynomial<double>> pk):context(std::move(context)),sampler(context),pk(std::move(pk)){}

 std::pair<Polynomial<double>, Polynomial<double>> CKKSEncryptor::encrypt(const Polynomial<double>& ptx){
    auto v=sampler.ZO();
    auto e0=sampler.DG();
    auto e1=sampler.DG();

    auto v_pk=std::make_pair(pk.first*v,pk.second*v+e1);
    auto ctx=std::make_pair(v_pk.first+e0+ptx,v_pk.second+e1);
    return ctx;
 }