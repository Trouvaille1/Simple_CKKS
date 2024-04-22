#include "QPolynomial.h"
#include <iostream>

using namespace std;

int main(){
    int N=4;
    int q=pow(2,16);

    QPolynomial::setRing(q,N);//全局设置环系统

    QPolynomial a({1,0,0,1});
    QPolynomial b({0,0,0,1});
    QPolynomial c=a+b;
    QPolynomial d=a*b;
    QPolynomial e=a/64.0;
    QPolynomial f=a-b;
    a.print();
    b.print();
    c.print();
    d.print();
    e.print();
    f.print();
}