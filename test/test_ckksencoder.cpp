//
// Created by hanggu on 24-4-17.
//
#include "CKKSEncoder.h"
#include "Polynomial.h"
#include "helper.h"
#include <iostream>

using namespace std;

int main() {

  int M = 8;
  double scale = 64.0;
  CKKSEncoder encoder(M, scale);
  cout<<endl<<endl<<endl;
  cout<<"sigma_R_basis:"<<endl;
  vector<vector<complex<double>>> sigma_R_basis = encoder.get_sigma_R_basis();
  for (const auto &row : sigma_R_basis) {
    for (const auto &elem : row) {
      cout << elem << " ";
    }
    cout << endl;
  }
  cout<<endl;

  // 消息
  vector<complex<double>> m1 = {1, 2, 3, 4};
  vector<complex<double>> m2 = {1, -2, 3, -4};

  // 直接使用sigma_inverse()编码
  Polynomial<complex<double>> p1 = encoder.sigma_inverse(m1);
  Polynomial<complex<double>> p2 = encoder.sigma_inverse(m2);
  print_line(__LINE__);
  cout << "p1=";
  p1.print();
  print_line(__LINE__);
  p2.print();

  Polynomial<complex<double>> p_add = p1 + p2;
  print_line(__LINE__);
  p_add.print();

  // 直接使用sigma()解码
  vector<complex<double>> p_add_decoded = encoder.sigma(p_add);
  print_line(__LINE__);
  print_vector(p_add_decoded);

  Polynomial<complex<double>> poly_modulo({1, 0, 0, 0, 1}); // 分圆多项式为x^4+1
  print_line(__LINE__);
  poly_modulo.print();

  Polynomial<complex<double>> p_mult = p1 * p2 % poly_modulo; // 时域相乘等于频域卷积
  print_line(__LINE__);
  p_mult.print();

  vector<complex<double>> p_mult_decoded = encoder.sigma(p_mult);
  print_line(__LINE__);
  print_vector(p_mult_decoded);

  // 测试输入向量（）
  std::vector<std::complex<double>> z = {std::complex<double>(3, 4),
                                         std::complex<double>(2, -1)};
  std::cout << "message:";
  for (const auto &elem : z) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;

  // 编码
  Polynomial<double> p = encoder.encode(z);
  std::cout << "encode:";
  p.print();

  // 解码
  std::vector<std::complex<double>> d = encoder.decode(p);
  std::cout << "decode:";
  for (const auto &elem : d) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;
}