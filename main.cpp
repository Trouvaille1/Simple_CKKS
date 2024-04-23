#include <iostream>
#include <utility>
#include <vector>
#include "CKKSDecryptor.h"
#include "CKKSEncoder.h"
#include "CKKSEncryptor.h"
#include "Context.h"
#include "Keygen.h"
#include "QPolynomial.h"

using namespace std;

int main() {
    int N=8;
    int M=N*2;
    int scale=pow(2,12);//scale=pow(2,小数部分精度)。注意,scale太小不行，结果会出错。太大了q会溢出，变成0(12是目前支持的最大值。如果要支持更大的scale，需要使用中国剩余定理CRT)
    vector<int>moduli={static_cast<int>(scale * pow(2,5)), scale, static_cast<int>(scale * pow(2,5))};//模链。需要提前设定好要进行L次乘法（这里我们设置L=1），设置q0=pow(2,小数精度位数+整数精度位数)=scale*pow(2,整数精度位数)，q=scale^L*q0

    Context context(N,moduli);
    Keygen keygen(context);

    auto sk=keygen.generate_secret_key();
    auto pk=keygen.generate_public_key(sk);
    cout<<"开始生成relin_key："<<endl;
    auto relin_key=keygen.generate_relin_key(sk);

    CKKSEncoder encoder(M,scale);
    CKKSEncryptor encryptor(context,pk);
    CKKSDecryptor decryptor(context,sk);
    
    // std::vector<std::complex<double>> z;
    // z.reserve(N / 2);
    // for (int i = 0; i < N / 2; ++i) {
    //     z.emplace_back(i, 0);
    // }
    // cout<<"原始消息z："<<endl;
    // for(auto elem:z){
    //     cout<<elem<<" ";
    // }
    // cout<<endl;

    // auto ptx=encoder.encode(z);//编码为明文（多项式）
    
    // auto ctx=encryptor.encrypt(ptx);//加密为密文
    // ptx=decryptor.decrypt(ctx);//解密为明文

    // auto z_decoded=encoder.decode(ptx);//解码消息
    // cout<<"解密后的消息z_decoded，应该与z几乎相同："<<endl;
    // for(auto elem:z_decoded){
    //     cout<<elem<<" ";
    // }
    // cout<<endl;

    // cout<<"正在进行密文相加..."<<endl;
    // auto ct_add=make_pair(ctx.first+ctx.first,ctx.second+ctx.second);//密文相加
    // auto ptx_add=decryptor.decrypt(ct_add);//解密为明文
    // auto z_add=encoder.decode(ptx_add);//解码消息
    // cout<<"解密后的消息："<<endl;
    // for(auto elem:z_add){
    //     cout<<elem<<" ";
    // }
    // cout<<endl;


    cout<<"正在进行密文相乘..."<<endl;
    int slot_count = encoder.get_slot_count();
    cout<<"slot_count:"<<slot_count<<endl;
    vector<complex<double>> a(slot_count, complex<double>(1.0, 0.0));
    vector<complex<double>> b(slot_count, complex<double>(5.0, 0.0));
    
    auto ptx_1=encoder.encode(a);
    auto ptx_2=encoder.encode(b);

    auto ctx_1=encryptor.encrypt(ptx_1);
    auto ctx_2=encryptor.encrypt(ptx_2);

    //对密文进行同态乘法
    auto d0=ctx_1.first*ctx_2.first;
    auto d1=ctx_1.second*ctx_2.first+ctx_1.first*ctx_2.second;
    auto d2=ctx_1.second*ctx_2.second;
    auto ctx_mul=make_pair(d0,make_pair(d1,d2));//三维多项式，(d0,d1,d2)

    //不使用重线性化，直接解密ctx_mul。解密公式为：DecryptMult(cmult,s)=d0+d1*s+d2*s^2
    auto ptx_mul=d0+d1*sk+d2*sk*sk;
    auto ptx_mul_decoded=encoder.decode(ptx_mul);
    //执行再缩减操作。当然，由于我们设置的L=1(即只使用一次乘法)，暂时还没实现模链更改
    for(auto &elem:ptx_mul_decoded){
        elem/=scale;
    }
    cout<<"不使用重线性化，直接解密ctx_mul："<<endl;
    for(auto elem:ptx_mul_decoded){
        cout<<elem<<" ";
    }
    cout<<endl;

    //重线性化有问题。
    //使用重线性化，解密ctx_mul。首先，对cmult（ctx_mul）重线性化，使之由三维变成两维。公式为：Relin(ctx_mul,evk)=Relin((d0,d1,d2),evk)=(d0,d1)+[p^-1*d2*evk](mod q)
    //首先，计算P=[p^-1*d2*evk](mod q)
    auto P=make_pair((relin_key.first/pow(2,12)*d2)%moduli[1], (relin_key.second/pow(2,12)*d2)%moduli[1]);

    //然后，计算Relin((d0,d1,d2),evk)=(d0,d1)+P
    auto ctx_relin=make_pair(d0+P.first,d1+P.second);
    //解密
    auto ptx_relin=decryptor.decrypt(ctx_relin);
    auto z_relin=encoder.decode(ptx_relin);
    //将消息除以scale得到最终消息
    for(auto &elem:z_relin){
        elem/=scale;
    }
    cout<<"使用重线性化，解密ctx_mul："<<endl;
    for(auto elem:z_relin){
        cout<<elem<<" ";
    }
    cout<<endl;





    // auto d=make_pair(ctx_mul.second.second*relin_key.first/context.get_special_prime(),ctx_mul.second.second*relin_key.second/context.get_special_prime());
    // d.first=Polynomial(encoder.coordinate_wise_random_rounding(d.first.getCoefficients()));
    // d.second=Polynomial(encoder.coordinate_wise_random_rounding(d.second.getCoefficients()));

    // auto ct_relin=make_pair(ctx_mul.first+d.first,ctx_mul.second.first+d.second);//乘法结果
    // auto ptx_mul=decryptor.decrypt(ct_relin);
    // auto z_mul=encoder.decode(ptx_mul);
    // //将消息除以scale得到最终消息
    // for(auto &elem:z_mul){
    //     cout<<"除以scale之前,elem="<<elem<<endl;
    //     // elem=complex<double>((static_cast<int>(elem.real())%moduli[1]), (static_cast<int>(elem.imag())%moduli[1]));
    //     elem/=scale;
        
    //     cout<<"除以scale之后,elem="<<elem<<endl;
    // }
    // cout<<"解密后的消息z_mul，应该与a*b几乎相同："<<endl;
    // for(auto elem:z_mul){
    //     cout<<elem<<" ";
    // }
    // cout<<endl;

}
