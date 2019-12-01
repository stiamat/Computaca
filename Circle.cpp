#include "Circle.h"

Circle::Circle(){

};

void Circle::print(){
    printf("Ciculo com Raio = %f Cor = (%.2f; %.2f; %.2f)\n", raio, corR, corG, corB);
};

int Circle::pointPerformanceCircle(){
    int raio = this->raio;
    if(raio >= 100){
        return 250;
    }else{
        return 2000/(raio*0.7);

    }
};

// Get's 
float Circle::get_raio(){
    return this->raio;
};
int Circle::get_pontosCirculo(){
    return this->pontosCirculo;
};
int Circle::get_id(){
    return this->id;
};

float Circle::get_x(){
    return this->x;
};
float Circle::get_y(){
    return this->y;
};

float Circle::get_corR(){
    return this->corR;
};
float Circle::get_corG(){
    return this->corG;
};
float Circle::get_corB(){
    return this->corB;
};

void Circle::set_raio(float raio){
    this->raio = raio;
};
void Circle::set_pontosCirculo(int pontosCirculo){
    this->pontosCirculo = pontosCirculo;
};
void Circle::set_id(int p){
    this->id = p;
};

void Circle::set_x(float p){
    this->x = p;
};
void Circle::set_y(float p){
    this->y = p;
};

void Circle::set_corR(float p){
    this->corR = p;
};
void Circle::set_corG(float p){
    this->corG = p;
};
void Circle::set_corB(float p){
    this->corB = p;
};
void Circle::setFriendshipCircle(int p){
    this->friends = p;
};
int Circle::isCircleEnemy(){
    return this->friends;
}