#ifndef _CIRCLE_H
#define _CIRCLE_H

#include <stdio.h>
#include <string>

class Circle{
    int id = 0;
    float raio = 0;
    float x = 0, y = 0;

    int pontosCirculo = 1000;
    float corR = 0, corG = 0, corB = 0;
    float corRSobre = 0, corGSobre = 0, corBSobre = 0;

    int friends = 1; //1 - inimigo; 2 - amigo;


public:
    float direcao = 0;
    float thetaCanhao = 0;
    float thetaHelice = 0;
    float inimigo_freqTiro = 0;
    float inimigo_vel = 0;
    float inimigo_vel_tiro = 0;
    int isInimigoVoador = 0;
    float xl = 0;
    float yl = 0;
    int colisao = 0;

    Circle();

    int pointPerformanceCircle();

    void print();

    float get_raio();
    int get_pontosCirculo();
    int get_id();
    void set_raio(float raio);
    void set_pontosCirculo(int pontosCirculo);
    void set_id(int p);

    float get_x();
    float get_y();
    void set_x(float p);
    void set_y(float p);


    float get_corR();
    float get_corG();
    float get_corB();
    void set_corR(float p);    
    void set_corG(float p);
    void set_corB(float p);

    void setFriendshipCircle(int p);
    int isCircleEnemy();


    void andaXCircle(float x)
    {
        this->set_x(this->get_x() + x);
    };

    void andaYCircle(float y)
    {
        this->set_y(this->get_y() + y);
    };


};

#endif
