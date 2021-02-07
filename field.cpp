#include <algorithm>
#include <iostream>
#include "field.h"


Field::Field(int w, int h){
    h_size = h, w_size = w;
    current = Matrix(w, h);
    old = Matrix(w, h);
}

Field::~Field(){
}

void Field::reset(){
    current.reset();
    old.reset();
}

void Field::swap(){
    std::swap(current.mat, old.mat);
}