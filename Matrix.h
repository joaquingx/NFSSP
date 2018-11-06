//
// Created by joaquin on 05/11/18.
//

#ifndef NFSSP_MATRIX_H
#define NFSSP_MATRIX_H
#include "globalConstants.h"
#include "iostream"
using namespace std;

constexpr int maxLength = 1000000;


struct Matrix{
    int realMatrix[maxLength];
    int rows,columns, elements;
    Matrix(const int & r, const int & c);
    Matrix();
    const auto size() const;
    const int& operator [](const int index) const;
    int& operator() (const int & a , const int & b);
    const int operator() (const int a, const int b) const;
    friend ostream& operator<<(ostream& os , const Matrix& obj);
};

#endif //NFSSP_MATRIX_H
