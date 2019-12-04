#ifndef _BALA_H
#define _BALA_H

#include "Circle.h"

class Bala
{
    float direcao = 0;
    float time = 0;
    float canhao = 0;
    float velocidadeBase = 0;
    float velocidade = 0;
    float velocidadeAviao = 0;
    float multVelocidade = 0;
    float raioOriginal = 0;
    int tipo = 1; //1 - tiro       2 - bomba
    float Iniy = 0, Inix = 0;
    

    Circle solido;

public:
    int balaIni = 0;

    Bala();

    void Desenha(GLint texte);
    void Atualiza(float p);

    int get_tipo();
    void set_tipo(int p);

    float get_time(){
        return this->time;
    }

    float get_direcao();
    void set_direcao(float p);

    float get_canhao();
    void set_canhao(float p);

    float get_velocidade();
    void set_velocidade(float p);

    float get_velocidadeAviao();
    void set_velocidadeAviao(float p);


    void set_multvelocidade(float p){
        this->multVelocidade = p;
    };

    float get_velocidadeBase();
    void set_velocidadeBase(float p);

    float get_x();
    void set_x(float p);

    float get_Iniy();
    void set_Iniy(float p);

    float get_Inix();
    void set_Inix(float p);

    float get_y();
    void set_y(float p);

    float get_z();
    void set_z(float p);

    float get_raio();
    void set_raio(float p);

    void andaXtiro(float x);
    void andaYtiro(float y);
};
#endif