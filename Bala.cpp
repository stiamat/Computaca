#include <GL/glut.h>

#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <unistd.h>

#include "Bala.h"

#define PI 3.1514

using namespace std;
GLUquadricObj *tiro = gluNewQuadric();
GLUquadricObj *bomba = gluNewQuadric();

Bala::Bala(){

};

void Bala::Desenha(GLint texte)
{
    if (this->tipo == 1)
    {
        GLfloat x, y;
        glColor3f(0, 0, 0);

        // glBegin(GL_POLYGON);
        // for (int ii = 0; ii < 360; ii++)
        // {
        //     x = this->solido.get_raio()/10 * cos(PI * ii / 180);
        //     y = this->solido.get_raio()/10 * sin(PI * ii / 180);
        //     glVertex3f(x, y, 0); // cria o vértice
        // }
        // glEnd();
        glPushMatrix();

            GLfloat materialEmission[] = { 0.10, 0.10, 0.10, 1};
            GLfloat materialColorA[] = { 0.5, 0.5, 0.5, 1};
            GLfloat materialColorD[] = { 1.0, 1.0, 1.0, 1};
            GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
            GLfloat mat_shininess[] = { 50.0 };
            glColor3f(0,0.5,0.5); //meu
        
            glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
            

            // glEnable(GL_TEXTURE_2D);
                GLUquadricObj *bola = gluNewQuadric();
                gluQuadricNormals(bola, GLU_SMOOTH);
                gluQuadricOrientation(bola,  GLU_INSIDE);
                gluQuadricTexture(bola, GL_TRUE);

                glBindTexture (GL_TEXTURE_2D, texte);

                gluSphere(tiro,this->solido.get_raio()/10, 10, 10);
                gluDeleteQuadric(bola);

            // glDisable(GL_TEXTURE_2D);
            
        glPopMatrix();
        
    }
    else
    {
        GLfloat x, y;
        glColor3f(0, 0, 0);

        // glBegin(GL_POLYGON);
        // for (int ii = 0; ii < 360; ii++)
        // {
        //     x = (this->solido.get_raio() * 0.3) * cos(PI * ii / 180);
        //     y = (this->solido.get_raio() * 0.3) * sin(PI * ii / 180);
        //     glVertex3f(x, y, 0); // cria o vértice
        // }
        // glEnd();
        gluSphere(bomba,this->solido.get_raio()*0.3, 10, 10);
    }
};

int Bala::get_tipo()
{
    return this->tipo;
};

void Bala::set_tipo(int p)
{
    this->tipo = p;
};

float Bala::get_direcao()
{
    return this->direcao;
};

void Bala::set_direcao(float p)
{
    this->direcao = p;
};

float Bala::get_canhao()
{
    return this->canhao;
};

void Bala::set_canhao(float p)
{
    this->canhao = p;
};

float Bala::get_velocidade()
{
    return this->velocidade;
};

void Bala::set_velocidade(float p)
{
    this->velocidade = p;
};

float Bala::get_velocidadeBase()
{
    return this->velocidadeBase;
};

void Bala::set_velocidadeBase(float p)
{
    this->velocidadeBase = p;
};

float Bala::get_x()
{
    return this->solido.get_x();
};
void Bala::set_x(float p)
{
    this->solido.set_x(p);
};

float Bala::get_z(){
    return this->solido.get_z();
};
void Bala::set_z(float p){
    this->solido.set_z(p);
};

float Bala::get_y()
{
    return this->solido.get_y();
};
void Bala::set_y(float p)
{
    this->solido.set_y(p);
};

float Bala::get_Iniy()
{
    return this->Iniy;
};

void Bala::set_Iniy(float p)
{
    this->Iniy = p;
};

float Bala::get_Inix()
{
    return this->Inix;
};

void Bala::set_Inix(float p)
{
    this->Inix = p;
};

float Bala::get_raio()
{
    return this->solido.get_raio();
};
void Bala::set_raio(float p)
{
    this->raioOriginal = p;
    this->solido.set_raio(p);
};

void Bala::Atualiza(float p)
{
    float desl;
    float multX;
    float multY;
    float newdesl;
    if (this->tipo == 1)
    {
        
        multX = cos((this->direcao - this->canhao) * PI / 180.0) * (this->multVelocidade+1);
        //cout << "X: " << multX << endl;
        multY = sin((this->direcao - this->canhao) * PI / 180.0) * (this->multVelocidade+1);
        //cout << "Y: " << multY << endl;
        newdesl = 0;

        // cout << this->solido.get_raio() << endl;
        if(this->multVelocidade+1 > 1.3){
            newdesl = (((get_velocidade()) * sqrt(2) * get_velocidadeBase() * -multX * p)/ 2);       

        }else{
            newdesl = (((get_velocidade() + 30) * sqrt(2) * get_velocidadeBase() * -multX * p)/ 2);       

        }
        // newdesl = (((get_velocidade() + 50) * sqrt(2) * get_velocidadeBase() * -multX * p)/ 2);       
        andaYtiro(newdesl);
        // cout <<"y:"<<  get_velocidade() << endl;


        if(this->multVelocidade+1 > 1.3){
            newdesl = (((get_velocidade()) * sqrt(2) * get_velocidadeBase() * -multY * p)/ 2);       

        }else{
            newdesl = (((get_velocidade() + 30) * sqrt(2) * get_velocidadeBase() * -multY * p)/ 2);       

        }
        // newdesl = (((get_velocidade() + 50) * sqrt(2) * get_velocidadeBase() * -multY * p)/ 2);
        andaXtiro(newdesl);
        // cout << "x:"<< get_velocidade() << endl;

    }
    else
    {
        this->time += p;
        if(this->time < 4.0){

            // desl = (get_velocidade() * p/ 2.0);//lembrar de pegar o vel do tiro
            // cout << get_velocidade() << endl;
            multX = cos((this->direcao) * PI / 180.0) * this->multVelocidade;
            //cout << "X: " << multX << endl;
            multY = sin((this->direcao) * PI / 180.0) * this->multVelocidade;
            //cout << "Y: " << multY << endl;
            newdesl = 0;

            float m = (1.0*((4.0-this->time)/4) + 1.0)/2.0;
            this->solido.set_raio(this->raioOriginal*m);
            // cout << this->solido.get_raio() << endl;
            newdesl = ((get_velocidade() * 1.3*sqrt(2) * -multX * p)/ 2);
            andaYtiro(newdesl);

            newdesl = ((get_velocidade() * 1.3*sqrt(2) * -multY * p)/ 2);
            andaXtiro(newdesl);
        }
        
    }
    
};

float Bala::get_velocidadeAviao(){
    return this->velocidadeAviao;
};
void Bala::set_velocidadeAviao(float p){
    this->velocidadeAviao = p;
};

void Bala::andaXtiro(float x)
{
    this->set_Inix(this->get_Inix() + x);
};

void Bala::andaYtiro(float y)
{
    this->set_Iniy(this->get_Iniy() + y);
};