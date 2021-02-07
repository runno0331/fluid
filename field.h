#pragma once
#include "matrix.h"

class Field{
private:
public:
    int h_size;
    int w_size;
    Matrix current, old;

    Field(int w=1, int h=1);
    ~Field();
    void reset();
    void swap();
};