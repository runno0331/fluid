#pragma once
#include "field.h"

class Fluid{
public:
    int w_size;
    int h_size;
    Field u;
    Field v;
    Field dense;
    Matrix div;
    Matrix p;
    Matrix u_source;
    Matrix v_source;
    Matrix dense_source;

    Fluid(int, int);
    void reset();
    double get(int x, int y);
    void draw_velocity();
    void draw();
    void add_source(Matrix* target, Matrix* source);
    void add_dense(int x, int y);
    void add_velocity(int x, int y, int vx, int vy);
    void set_bound(int bound_flag, Matrix* x);
    void diffuse(int bound_flag, Field* x, double diff_coef);
    void advect(int bound_flag, Field* x, Matrix* u_mat, Matrix* v_mat);
    void project();
    void dens_step();
    void vel_step();
    void clean_source();
    void simulate_single_step();
};