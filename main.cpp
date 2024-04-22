#include <iostream>
#include <utility>
#include <vector>
#include "CKKSDecryptor.h"
#include "CKKSEncoder.h"
#include "CKKSEncryptor.h"
#include "Context.h"
#include "Keygen.h"

using namespace std;

int main() {
    int N=4;
    int M=N*2;
    int scale=pow(2,6);//太大了q会溢出，变成0
    vector<int>moduli={static_cast<int>(scale * pow(2,5)), scale, static_cast<int>(scale * pow(2,5))};//模链

    Context context(N,moduli);
    Keygen keygen(context);

    auto sk=keygen.generate_secret_key();
    auto pk=keygen.generate_public_key(sk);
    auto relin_key=keygen.generate_relin_key(sk);

    CKKSEncoder encoder(M,scale);
    CKKSEncryptor encryptor(context,pk);
    CKKSDecryptor decryptor(context,sk);
    
    std::vector<std::complex<double>> z;
    z.reserve(N / 2);
    for (int i = 0; i < N / 2; ++i) {
        z.emplace_back(i, 0);
    }
    cout<<"原始消息z："<<endl;
    for(auto elem:z){
        cout<<elem<<" ";
    }
    cout<<endl;

    auto ptx=encoder.encode(z);//编码为明文（多项式）
    
    //加解密有问题
    // auto ctx=encryptor.encrypt(ptx);//加密为密文
    // ptx=decryptor.decrypt(ctx);//解密为明文

    auto z_decoded=encoder.decode(ptx);//解码消息
    cout<<"解密后的消息z_decoded，应该与z几乎相同："<<endl;
    for(auto elem:z_decoded){
        cout<<elem<<" ";
    }
    cout<<endl;

    // cout<<"正在进行密文相加..."<<endl;
    // auto ct_add=make_pair(ctx.first+ctx.first,ctx.second+ctx.second);//密文相加
    // auto ptx_add=decryptor.decrypt(ct_add);//解密为明文
    // auto z_add=encoder.decode(ptx_add);//解码消息
    // cout<<"解密后的消息："<<endl;
    // for(auto elem:z_add){
    //     cout<<elem<<" ";
    // }
    // cout<<endl;








}
