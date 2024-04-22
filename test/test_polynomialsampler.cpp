#include "Context.h"
#include "PolynomialSampler.h"
using namespace std;

int main(){
    int N=16;//要大于64，因为设置了h=64
    int M=N*2;
    int scale=pow(2,3);//太大了q会溢出，变成0
    vector<int>moduli={static_cast<int>(scale * pow(2,5)), scale, static_cast<int>(scale * pow(2,5))};//模链
    Context context(N,moduli);
    PolyNomialSampler sampler(context);
    auto p1=sampler.DG();
    auto p2=sampler.UniformPolynomial();
    auto p3=sampler.ZO();
    auto p4=sampler.HWT();
    cout<<"DG为：" <<endl;
    p1.print();
    cout<<"UniformPolynomial为：" <<endl;
    p2.print();
    cout<<"ZO为：" <<endl;
    p3.print();
    cout<<"HWT为：" <<endl;
    p4.print();
}