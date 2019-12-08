#ifndef _ARENA_H
#define _ARENA_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>

#include "Circle.h"
#include "Line.h"
#include "Bala.h"

using namespace std;

class Arena
{
    //-----------
    

    //------------

    char *nome;

    // Definição de Arena
    Circle arena_config;

    //---------------------

    // Definição de Jogador
    Circle jogador_config;
    
    float direcao = 0;
    float thetaCanhao = 0;
    float thetaCanhaoZ = 0;
    float thetaHelice = 0;

    float raioOriginalJogador = 0;
    float velocidadeJogadorBase = 0, velocidadeJogadorAtual = 0, velocidadeJogadorXAtual = 0, velocidadeJogadorYAtual = 0;
    float aceleracao_x = 0, aceleracao_y = 0;
    float deslAtualX = 0, deslX = 0;
    float deslAtualY = 0, deslY = 0;
    float velocidadeTiroBase = 0;
    int decolagem = 0;

    vector<Bala *> tiros;
    //----------------------
    
    //inimigo
    
    float tempo_tiro_Inimi = 0;
    //--------------------

    // Definição de Individuos
    vector<Circle *> individuos;
    //-------------------------

    // Definição de Decolagem
    Line pista_decolagem;
    //------------------------

    //Variaveis auxiliares
    float timeOld = 0, timeNew = 0, deltaT = 0, deltaS = 0;

    //------------------------
private:
    
    void Desenha_Arena(Circle *arena, Circle *jogador, vector<Circle *> *lista_individuos, Line *pista);
    void Desenha_Chao();
    void Desenha_Ceu();
    void DesenhaCilindro(GLuint text);
    void Desenha_Esfera(int raio, GLuint text);
    void Desenha_Cubo(int raio, GLuint text);
    void Desenha_Individuos(vector<Circle *> *lista_individuos);
    void DesenhaBaseInimiga(vector<Circle *> *lista_individuos);
    void Desenha_Pista(float x1, float x2, float y1, float y2, float corR, float corG, float corB);
    void Desenha_Jogador(int ini, float x, float y, float z, float raio, float thetaCanhao, float thetaHelice, float direcao, float direcaoZ, float thetaCanhaoZ);
    void Desenha_Circulo(float raio, float r, float g, float b);
    void Desenha_Retangulo(float height, float width, float r, float g, float b);
    void Desenha_Triangulo(float tam, float r, float g, float b);
    void Desenha_Tiro();
    void Desenha_Parede();
    void Aceleracao();
    void DesenhaTexto(char *string);
    int encostandoNumInimigo();

public:
    GLuint textureParedes;
    GLuint textureBaseInimiga;
    GLuint textureChao;
    GLuint textureCeu;
    GLuint textureAviao;
    GLuint textureAviaoIni;
    GLuint textureBala;

    float inimigo_freqTiro;
    float inimigo_vel;
    float inimigo_vel_tiro;
    float raioBase;
    //Arena
    Arena();

    GLuint LoadTextureRAW( const char * filename );
    void inputProcessing(char *input);
    void add_individuos(int x, int y, int raio, float r, float g, float b, int id);
    void set_partida(int x1, int y1, int x2, int y2, float r, float g, float b, int id);
    Circle get_jogador()
    {
        return jogador_config;
    };
    void print();

    float ortho_Config(int coord, int maior_menor)
    {
        //coord -> [1 x]  [2 y]
        if (coord == 1)
        {
            if (maior_menor == -1)
            {
                return arena_config.get_x() - arena_config.get_raio();
            }
            else
            {
                return arena_config.get_x() + arena_config.get_raio();
            }
        }
        if (coord == 2)
        {
            if (maior_menor == -1)
            {
                return arena_config.get_y() - arena_config.get_raio();
            }
            else
            {
                return arena_config.get_y() + arena_config.get_raio();
            }
        }
    }

    void reset(Arena x)
    {
        this->aceleracao_x = ((this->pista_decolagem.get_x2() - this->pista_decolagem.get_x1()) * 2) / 16.0;
        this->aceleracao_y = ((this->pista_decolagem.get_y2() - this->pista_decolagem.get_y1()) * 2) / 16.0;
        velocidadeJogadorBase = 1;
        velocidadeJogadorAtual = sqrt(pow((aceleracao_x*4.0), 2) + pow((aceleracao_y*4.0), 2));
        velocidadeJogadorXAtual = 0, velocidadeJogadorYAtual = 0;
        deslAtualX = 0, deslX = 0;
        deslAtualY = 0, deslY = 0;
        decolagem = 0;
        timeOld = 0, timeNew = 0, deltaT = 0, deltaS = 0;
        Circle xx = x.get_jogador();

        this->jogador_config.set_raio(xx.get_raio());
        raioOriginalJogador = xx.get_raio();

        this->jogador_config.set_id(xx.get_id());
        this->jogador_config.set_x(xx.get_x());
        this->jogador_config.set_y(xx.get_y());
        this->jogador_config.set_corR(xx.get_corR());
        this->jogador_config.set_corG(xx.get_corG());
        this->jogador_config.set_corB(xx.get_corB());

        float auxX = pista_decolagem.get_x2() - jogador_config.get_x(), auxY = pista_decolagem.get_y2() - jogador_config.get_y();
        this->direcao = anguloJogador(auxX, auxY);
        this->jogador_config.direcao = anguloJogador(auxX, auxY);
        this->jogador_config.direcaoZ = 0;

        this->tiros.erase(this->tiros.begin(), this->tiros.end());
        this->individuos.erase(this->individuos.begin(), this->individuos.end());

        this->inimigo_freqTiro = x.inimigo_freqTiro;
        this->inimigo_vel = x.inimigo_vel;
        this->inimigo_vel_tiro = x.inimigo_vel_tiro;

        Circle *ind;
        for (int i = 0; i < x.individuos.size(); i++)
        {
            ind = x.individuos[i];
            add_individuos(ind->get_x(), ind->get_y(), ind->get_raio(), ind->get_corR(), ind->get_corG(), ind->get_corB(), ind->get_id());
        }
    };

    //-------------------------

    int getDecolagem()
    {
        return decolagem;
    };

    void addEstadoDecolagem()
    {
        decolagem++;
    };

    float distanciaJogadorFimPista()
    {
        return sqrt(pow(jogador_config.get_x() - pista_decolagem.get_x2(), 2) + pow(jogador_config.get_y() - pista_decolagem.get_y2(), 2));
    };

    void colocaAviaoNosEixo(float x, float y, float direcao);
    void inicioDecolagem();
    void decolando();
    void trocaDeContexto();
    void decolou(float deltaT);
    void andaXjogador(float x);
    void andaYjogador(float x);
    void andaZjogador(float z);
    float deslocX(float deltaT);
    float deslocY(float deltaT);
    float anguloJogador(float x, float y);
    float multiplicadorDeslocamentoY(float angulo);
    float multiplicadorDeslocamentoX(float angulo);
    void curvaAviao(float a)
    {
        this->direcao += a * velocidadeJogadorBase;
        jogador_config.direcao = this->direcao;
    };
    void aviaoZ(float angulo){
        this->jogador_config.direcaoZ += angulo * velocidadeJogadorBase;
        float ind = 10;
        if(this->jogador_config.direcaoZ >= ind){
            this->jogador_config.direcaoZ = ind;
        }

        if(this->jogador_config.direcaoZ <= -ind){
            this->jogador_config.direcaoZ = -ind;
        }

    };

    void machaVelocidade(float a)
    {
        if (this->velocidadeJogadorBase >= 0)
        {
            this->velocidadeJogadorBase += a;
            this->inimigo_vel += a;
        }
        else
        {
            this->velocidadeJogadorBase = 0;
            this->inimigo_vel = 0;
        }
    };

    void giraHelice()
    {
        float mult = 50;
        if (getDecolagem() <= 2)
        {
            this->thetaHelice += ((velocidadeJogadorXAtual + velocidadeJogadorYAtual) * velocidadeJogadorBase * mult / 360) + (velocidadeJogadorXAtual + velocidadeJogadorYAtual) * mult / 360;
        }
        if (getDecolagem() == 3)
        {
            this->thetaHelice += (((velocidadeJogadorAtual * velocidadeJogadorBase) * mult) / 360) + ((velocidadeJogadorAtual * mult) / 360);
        }
    };

    void miraCanhao(float x)
    {
        this->thetaCanhao += x;
        

        if (this->thetaCanhao < -30.0)
        {
            this->thetaCanhao = -30.0;
        }
        if (this->thetaCanhao > 30.0)
        {
            this->thetaCanhao = 30.0;
        }
        this->jogador_config.thetaCanhao += x;
    };

    void miraCanhaoZ(float z){
        this->jogador_config.thetaCanhaoZ += z;

        if (this->jogador_config.thetaCanhaoZ < -30.0)
        {
            this->jogador_config.thetaCanhaoZ = -30.0;
        }
        if (this->jogador_config.thetaCanhaoZ > 30.0)
        {
            this->jogador_config.thetaCanhaoZ = 30.0;
        }
    }

    void tiro(int tipo);
    void tiroInimigo(Circle* ind);
    void limpaTiros();
    void atualizaTiros(float p);
    void atualizaInimigos(float p);

    int saindoMeioArena(Circle* p);
    int saindoPontaArena(Circle* p);
    float x0 = 0,y0 = 0, flag = 0;
    void TrataForaDaArena(Circle* p);

    int BasesInimigas(){
        int aux = 0;
        for (int i = 0; i < this->individuos.size(); i++)
        {
            Circle* ind = this->individuos[i];
            if(ind->get_corG() != 0) aux++;
        }
        if(aux == 0){
            addEstadoDecolagem();
        }
        return aux;
    };

    //-------------------------

    void Desenha()
    {
        Desenha_Arena(&arena_config, &jogador_config, &individuos, &pista_decolagem);
    };
};

#endif