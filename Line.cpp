#include <vector>
#include <string.h>
#include "Line.h" 

using namespace std;

Line::Line(){
    //Inicio da Pista
    x1 = 0;
    y1 = 0;

    //Fim da Pista
    x2 = 0;
    y2 = 0;

    //Cor da Pista
    r = 0;
    g = 0;
    b = 0;

    //Identificador da Pista
    id = 0;
};

float Line::get_x1(){
    return this->x1;
};
void Line::set_x1(float p){
    this->x1 = p;
};


float Line::get_y1(){
    return this->y1;
};
void Line::set_y1(float p){
    this->y1 = p;
};


float Line::get_x2(){
    return this->x2;
};
void Line::set_x2(float p){
    this->x2 = p;
};


float Line::get_y2(){
    return this->y2;
};
void Line::set_y2(float p){
    this->y2 = p;
};


float Line::get_r(){
    return this->r;
};
void Line::set_r(float p){
    this->r = p;
};


float Line::get_g(){
    return this->g;
};
void Line::set_g(float p){
    this->g = p;
};


float Line::get_b(){
    return this->b;
};
void Line::set_b(float p){
    this->b = p;
};


int Line::get_id(){
    return this->id;
};
void Line::set_id(int p){
    this->id = p;
};