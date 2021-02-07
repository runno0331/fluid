#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <random>
#include <iostream>
#include <GL/glut.h>
#include "fluid.h"

#define WINDOW_X (400)
#define WINDOW_Y (400)
#define WINDOW_NAME "test1"

void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);
void glut_specialkey(int key, int x, int y);
void glut_idle();

void draw();
void draw2();
void draw_dot(double x, double y, double s, double color);
void draw_square(double x, double y, double w, double h, double color);

void set_boundary(int, int);

int g_display_mode = 1;

bool g_isLeftButtonOn = false;

int polygon_n = 10;
int prev_x = -1, prev_y = -1;
bool vel_flag = false, prev_init = false;

Fluid fluid(WINDOW_X, WINDOW_Y);

std::random_device rnd;

int main(int argc, char *argv[]){
    init_GL(argc, argv);
    init();
    set_callback_functions();
    glutMainLoop();
    
    return 0;
}

void init_GL(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(WINDOW_X, WINDOW_Y);
    glutCreateWindow(WINDOW_NAME);
}

void init(){
    glClearColor(0.0, 0.0, 0.0, 0.0);
} 

void set_callback_functions(){
    glutDisplayFunc(glut_display);
    glutKeyboardFunc(glut_keyboard);
    glutMouseFunc(glut_mouse);
    glutMotionFunc(glut_motion);
    glutSpecialFunc(glut_specialkey);
    glutIdleFunc(glut_idle);
}

// call backfunction for keyboard
void glut_keyboard(unsigned char key, int x, int y){
    switch(key){

    case 'q':
    case 'Q':
    case '\033': // Esc key
        exit(0);
    case '1':
        g_display_mode = 1;
        break;
    case '2':
        g_display_mode = 2;
        break;
    case 'r':
        fluid.reset();
        break;
    case 'd':
        vel_flag = false;
        break;
    case 'v':
        vel_flag = true;
        prev_init = false;
        break;
    }
    glutPostRedisplay();
}

void glut_display(){
    glClear(GL_COLOR_BUFFER_BIT);
    
    switch(g_display_mode){
    case 1:
        draw();
        break;
    case 2:
        draw2();
        break;
    }
    glFlush();
}

void glut_mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_UP){
            g_isLeftButtonOn = false;
            prev_init = false;
        }else if(state == GLUT_DOWN){
            g_isLeftButtonOn = true;
            prev_init = true;
            prev_x = x;
            prev_y = y;
        }
    }

    // if(button == GLUT_RIGHT_BUTTON){
    //     if(state == GLUT_UP){
    //         g_isRightButtonOn = false;
    //     }else if(state == GLUT_DOWN){
    //         g_isRightButtonOn = true;
    //     }
    // };
    glutPostRedisplay();
}

void glut_motion(int x, int y){
    if(0 <= x && x < WINDOW_X && 0 <= y && y <= WINDOW_Y){
        if(g_isLeftButtonOn){
            if(vel_flag){
                if(prev_init){
                    fluid.add_velocity(x, y, x-prev_x, -y+prev_y);    
                }
                prev_x = x;
                prev_y = y;
                prev_init = true;
            }else{
                fluid.add_dense(x, y);
            }
        }
    }
    glutPostRedisplay();
}

void glut_specialkey(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
        polygon_n++;
        break;

        case GLUT_KEY_DOWN:
        polygon_n--;
        if(polygon_n < 3) polygon_n = 3;
        break;
    }

    glutPostRedisplay();
}

void glut_idle(){
    glutPostRedisplay();
}

void draw(){
    fluid.simulate_single_step();
    fluid.draw();
}

void draw2(){
    fluid.simulate_single_step();
    fluid.draw();
    fluid.draw_velocity();
}
