#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include "fluid.h"

double dt = 0.1;
int solution_iteration = 5;
double diffusion_coef = 0.000001;
double viscocity_coef = 0.;
double user_velocity = 5;
double user_density = 10;

// constructor
Fluid::Fluid(int x, int y){
    if(x < 1 || y < 1) std::exit(1);

    int w = x+2, h = y+2;
    w_size = w, h_size = h;
    u = Field(w, h);
    v = Field(w, h);
    dense = Field(w, h);
    div = Matrix(w, h);
    p = Matrix(w, h);
    u_source = Matrix(w, h);
    v_source = Matrix(w, h);
    dense_source = Matrix(w, h);
}

// initialize value to 0
void Fluid::reset(){
    u.reset();
    v.reset();
    dense.reset();
    div.reset();
    p.reset();
    u_source.reset();
    v_source.reset();
    dense_source.reset();
}

// get density
double Fluid::get(int x, int y){
    return dense.current.mat[x][y];
}

void Fluid::draw_velocity(){
    double dw = 2.0 / double(w_size-2);
    double dh = 2.0 / double(h_size-2);
    for(int i=1; i<w_size-1; i+=5){
        for(int j=1; j<h_size-1; j+=5){
            double u_value = std::max(std::min(u.current.mat[i][j]*100.0, 10.0), -10.0) / 10.0;
            double v_value = std::max(std::min(v.current.mat[i][j]*100.0, 10.0), -10.0) / 10.0;
            if(std::abs(u_value) > 1e-3 || std::abs(v_value) > 1e-3){
                glBegin(GL_LINES);
                double x = (double(i) - 0.5) * dw - 1.0;
                double y = (double(j) - 0.5) * dh - 1.0;
                glColor3d(std::abs(u_value), std::abs(v_value), 0.0);
                glVertex2d(x, y);
                glColor3d(std::abs(u_value), std::abs(v_value), 0.0);
                glVertex2d(x+5.0*dw*u_value, y+5.0*dh*v_value);
                glEnd();
            }
        }
    }
}

void Fluid::draw(){
    double dw = 2.0 / double(w_size-2);
    double dh = 2.0 / double(h_size-2);
    glBegin(GL_POINTS);
    for(int i=0; i<w_size; i++){
        for(int j=0; j<h_size; j++){
            double color = dense.current.mat[i][j];
            color = std::max(0., std::min(1., color));
            double x = dw * double(i) - 1.0, y = dh * double(j) - 1.0;
            glColor3d(color, color, color);
            glVertex2d(x, y);
        }
    }
    glEnd();
}

void Fluid::add_source(Matrix* target, Matrix* source){
    for(int i=1; i<w_size-1; i++){
        for(int j=1; j<h_size-1; j++){
            target->mat[i][j] += dt * source->mat[i][j];
            if(source->mat[i][j] < -10){
            }
        }
    }
}

void Fluid::add_dense(int x, int y){
    int size=2;
    for(int i=-size; i<=size; i++){
        for(int j=-size; j<=size; j++){
            int temp_x = x+i, temp_y = h_size-y+j+1;
            if(0 < temp_x && temp_x < w_size-1 && 0 < temp_y && temp_y < h_size-1){
                dense_source.mat[temp_x][temp_y] = user_density;
            }
        }
    }
}

void Fluid::add_velocity(int x, int y, int vx, int vy){
    int size=2;
    for(int i=-size; i<=size; i++){
        for(int j=-size; j<=size; j++){
            int temp_x = x+i, temp_y = h_size-y+j+1;
            if(0 < temp_x && temp_x < w_size-1 && 0 < temp_y && temp_y < h_size-1){
                u_source.mat[temp_x][temp_y] = user_velocity * double(vx);
                v_source.mat[temp_x][temp_y] = user_velocity * double(vy);
            }
        }
    }
}

void Fluid::set_bound(int bound_flag, Matrix* x){
    // TODO add more conditions
    for(int i=1; i<w_size-1; i++){            
        x->mat[0][i]        = bound_flag==1 ? -x->mat[1][i]        : x->mat[1][i];
        x->mat[w_size-1][i] = bound_flag==1 ? -x->mat[w_size-2][i] : x->mat[w_size-2][i];
        x->mat[i][0]        = bound_flag==2 ? -x->mat[i][1]        : x->mat[i][1];
        x->mat[i][h_size-1] = bound_flag==2 ? -x->mat[i][h_size-2] : x->mat[i][h_size-2];
    }
    x->mat[0][0]               = (x->mat[1][0]               + x->mat[0][1])               / 2.0;
    x->mat[0][h_size-1]        = (x->mat[1][h_size-1]        + x->mat[0][h_size-2])        / 2.0;
    x->mat[w_size-1][0]        = (x->mat[w_size-2][0]        + x->mat[w_size-1][1])        / 2.0;
    x->mat[w_size-1][h_size-1] = (x->mat[w_size-2][h_size-1] + x->mat[w_size-1][h_size-2]) / 2.0;
}

void Fluid::diffuse(int bound_flag, Field* x, double diff_coef){
    double a = dt * diff_coef * double((w_size-2) * (h_size-2)) / 4.0;
    for(int k=0; k<solution_iteration; k++){
        for(int i=1; i<w_size-1; i++){
            for(int j=1; j<h_size-1; j++){
                x->current.mat[i][j] = (x->old.mat[i][j] + a*(x->current.mat[i-1][j] + x->current.mat[i+1][j] + x->current.mat[i][j-1] + x->current.mat[i][j+1])) / (1.0 + 4.0 * a);
            }
        }
    }
    set_bound(bound_flag, &x->current);
}

void Fluid::advect(int bound_flag, Field* x, Matrix* u_mat, Matrix* v_mat){
    double dt0 = dt * double(w_size-2);
    for(int i=1; i<w_size-1; i++){
        for(int j=1; j<h_size-1; j++){
            double px = double(i) - dt0 * u_mat->mat[i][j];
            double py = double(j) - dt0 * v_mat->mat[i][j];
            if(px < 0.5) px = 0.5; 
            if(px > double(w_size)-1.5) px = double(w_size) - 1.5;
            if(py < 0.5) py = 0.5; 
            if(py > double(h_size)-1.5) py = double(h_size) - 1.5;
            int i0 = floor(px), j0 = floor(py);
            int i1 = i0 + 1, j1 = j0 + 1;
            double s1 = px - double(i0), t1 = py - double(j0);
            double s0 = 1.0 - s1, t0 = 1.0 - t1;
            x->current.mat[i][j] = s0*(t0*x->old.mat[i0][j0] + t1*x->old.mat[i0][j1])+
                                   s1*(t0*x->old.mat[i1][j0] + t1*x->old.mat[i1][j1]);
        }
    }
    set_bound(bound_flag, &x->current);
}

void Fluid::project(){
    for(int i=1; i<w_size-1; i++){
        for(int j=1; j<h_size-1; j++){
            div.mat[i][j] = (u.current.mat[i+1][j] - u.current.mat[i-1][j] + v.current.mat[i][j+1] - v.current.mat[i][j-1]) / double(w_size-2);
            p.mat[i][j] = 0.0;
        }
    }
    set_bound(0, &div);
    set_bound(0, &p);

    for(int i=0; i<w_size; i++){
        for(int j=0; j<h_size; j++){
            if(p.mat[i][j] != 0.){ std::cout << "1\n"; std::exit(1);}
        }
    }
    for(int k=0; k<solution_iteration; k++){
        for(int i=1; i<w_size-1; i++){
            for(int j=1; j<h_size-1; j++){
                p.mat[i][j] = (-div.mat[i][j] + p.mat[i-1][j] + p.mat[i+1][j] + p.mat[i][j-1] + p.mat[i][j+1]) / 4.0;
            }
        }
        set_bound(0, &p);
    }
    for(int i=1; i<w_size-1; i++){
        for(int j=1; j<h_size-1; j++){
            u.current.mat[i][j] -= double(w_size-2) * (p.mat[i+1][j] - p.mat[i-1][j]) / 4.0;
            v.current.mat[i][j] -= double(h_size-2) * (p.mat[i][j+1] - p.mat[i][j-1]) / 4.0;
        }
    }
    set_bound(1, &u.current);
    set_bound(2, &v.current);
}

void Fluid::dens_step(){
    add_source(&dense.current, &dense_source);
    dense.swap();
    diffuse(0, &dense, diffusion_coef);
    dense.swap();
    advect(0, &dense, &u.current, &v.current);
}

void Fluid::vel_step(){
    add_source(&u.current, &u_source);
    add_source(&v.current, &v_source);
    u.swap();
    v.swap();
    diffuse(1, &u, viscocity_coef);
    diffuse(2, &v, viscocity_coef);
    project();
    u.swap();
    v.swap();
    advect(1, &u, &u.old, &v.old);
    advect(2, &v, &u.old, &v.old);
    project();
}

void Fluid::clean_source(){
    u_source.reset();
    v_source.reset();
    dense_source.reset();
}

void Fluid::simulate_single_step(){
    vel_step();
    dens_step();
    clean_source();
}
