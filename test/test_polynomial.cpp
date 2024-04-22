//
// Created by hanggu on 24-4-16.
//

#include "Polynomial.h"
#include <iostream>

// #include "Polynomial.cpp"
using namespace std;

int main() {
  
  Polynomial<int> poly1({1, 2, 0, 0, 3}); // 3x^4 + 2x + 1
  Polynomial<int> poly2({1, 0, 1});       // x^2+1
  Polynomial<int> poly3({1, 0, 0, 0, 1}); // x^4+1

  std::cout << "Polynomial 1: ";
  poly1.print();
  std::cout << "Polynomial 2: ";
  poly2.print();
  

  Polynomial<int> sum = poly1 + poly2;
  std::cout << "Sum: ";
  sum.print();

  Polynomial<int> product = poly1 * poly2 % poly3;
  std::cout << "Product: ";
  product.print();

  Polynomial<int> remainder = poly1 % poly2;
  std::cout << "Remainder of poly1 / poly2: ";
  remainder.print();
}