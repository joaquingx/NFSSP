//
// Created by joaquin on 05/11/18.
//

#include "Matrix.h"
#include <stdexcept>
#include "iostream"
using namespace std;

Matrix::Matrix(const int &r, const int &c): rows{r}, columns{c}{
    if(rows == 0 || columns == 0){
        throw out_of_range("Index is out of range on Matrix Constructor Class");
    }
    elements = rows * columns;
}

const auto Matrix::size() const {
    return elements;
}

const int &Matrix::operator[](const int index) const {
    return realMatrix[index];
}

int &Matrix::operator()(const int &a, const int &b) {
    if(a < 0  or a >= rows or b < 0 or b >= columns){
        throw out_of_range("Index is out of range on Matrix Class");
    }
    return realMatrix[a*columns + b];
}

const int Matrix::operator()(const int a, const int b) const {
    if(a < 0  or a > rows or b < 0 or b > columns){
        throw out_of_range("Index is out of range on Matrix Class");
    }
    return realMatrix[a*columns + b];
}

ostream& operator<<(ostream& os , const Matrix& obj){
    auto sz = obj.size();
    cout << obj.rows << "\t" << obj.columns << "\n";
    for(int i = 0; i < sz ; ++i){
        cout << obj[i] << "\t";
        if(!((i+1) % obj.columns)){
            cout << "\n";
        }
    }
}

Matrix::Matrix() {

}
