#include <cstdlib>
#include <iostream>
#include "matrix.h"

Matrix::Matrix(int w, int h){
    if(h < 1 || w < 1) std::exit(1);

    h_size = h; w_size = w;
    mat = new double*[w];
    for(int i=0; i<w; i++){
        mat[i] = new double[h];
    }
}

Matrix::Matrix(const Matrix &cp){
    h_size = cp.h_size, w_size = cp.w_size;
    mat = new double*[w_size];
    for(int i=0; i<w_size; i++){
        mat[i] = new double[h_size];
    }
    for(int i=0; i<w_size; i++){
        for(int j=0; j<h_size; j++){
            mat[i][j] = cp.mat[i][j];
        }
    }
}

// TODO: 何故かこれをコメントアウトするとdouble free errorが出る
Matrix::~Matrix(){
}

void Matrix::reset(){
    for(int i=0; i<w_size; i++){
        for(int j=0; j<h_size; j++){
            mat[i][j] = 0.0;
        }
    };
}