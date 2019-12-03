#include <GL/glut.h>

#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "tinyxml/tinyxml.h"
#include "imageloader.h"
#include "Arena.h"

#define PI 3.1415


using namespace std;

GLUquadricObj *chao = gluNewQuadric();
GLUquadricObj *ceu = gluNewQuadric();
GLUquadricObj *cilindro = gluNewQuadric();
GLUquadricObj *baseInimiga = gluNewQuadric();


Arena::Arena(){
    this->textureBaseInimiga = LoadTextureRAW("stars1.bmp");
    this->textureChao = LoadTextureRAW("stars1.bmp");
    this->textureCeu = LoadTextureRAW("stars1.bmp");
    this->textureParedes = LoadTextureRAW("stars1.bmp");
    this->textureAviao = LoadTextureRAW("stars1.bmp");
};

// --- Texture

GLuint Arena::LoadTextureRAW( const char * filename )
{

    GLuint texture;
    
    Image* image = loadBMP(filename);

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                             0,                            //0 for now
                             GL_RGB,                       //Format OpenGL uses for image
                             image->width, image->height,  //Width and height
                             0,                            //The border of the image
                             GL_RGB, //GL_RGB, because pixels are stored in RGB format
                             GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                               //as unsigned numbers
                             image->pixels);               //The actual pixel data
    delete image;

    return texture;
}

// GLuint textureBaseInimiga = LoadTextureRAW("stars1.bmp");
// GLuint textureChao = LoadTextureRAW("stars1.bmp");
// GLuint textureCeu = LoadTextureRAW("stars1.bmp");
// GLuint textureParedes = LoadTextureRAW("stars1.bmp");
// GLuint textureAviao;

void DisplayBaseinimiga (GLuint texture)
{
    GLfloat materialEmission[] = { 0.10, 0.10, 0.10, 1};
    GLfloat materialColorA[] = { 0.2, 0.2, 0.2, 1};
    GLfloat materialColorD[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_shininess[] = { 100.0 };
    glColor3f(0.5,0.5,0);
 
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBindTexture (GL_TEXTURE_2D, texture);
    
    // glBegin (GL_TRIANGLE_STRIP);
    // for ( int i = 0; i <objEarth->numVtx; i++)
    // {
    //     glNormal3f(objEarth->vtx[i].nX, objEarth->vtx[i].nY, objEarth->vtx[i].nZ);
    //     glTexCoord2f (objEarth->vtx[i].U, objEarth->vtx[i].V);
    //     glVertex3f (objEarth->vtx[i].X, objEarth->vtx[i].Y, objEarth->vtx[i].Z);
    // }
    // glEnd();
}
// ---


void Arena::add_individuos(int x, int y, int raio, float r, float g, float b, int id)
{
    srand(x);
    Circle *aux = new Circle;
    aux->set_x(x);
    aux->set_y(y);
    aux->set_raio(raio);
    aux->set_corR(r);
    aux->set_corG(g);
    aux->set_corB(b);
    aux->set_id(id);
    if(g == 0){
        aux->isInimigoVoador = 1;
        aux->inimigo_freqTiro = this->inimigo_freqTiro;
        aux->inimigo_vel = this->inimigo_vel;
        aux->inimigo_vel_tiro = this->inimigo_vel_tiro;
        aux->direcao = 0;
        aux->thetaCanhao = 0;
    }
    individuos.push_back(aux);
};

void Arena::set_partida(int x1, int y1, int x2, int y2, float r, float g, float b, int id)
{
    this->pista_decolagem.set_x1(x1);
    this->pista_decolagem.set_y1(y1);

    this->pista_decolagem.set_x2(x2);
    this->pista_decolagem.set_y2(y2);

    this->pista_decolagem.set_r(r);
    this->pista_decolagem.set_g(g);
    this->pista_decolagem.set_b(b);

    this->pista_decolagem.set_id(id);

    this->aceleracao_x = ((this->pista_decolagem.get_x2() - this->pista_decolagem.get_x1()) * 2) / 16.0;
    this->aceleracao_y = ((this->pista_decolagem.get_y2() - this->pista_decolagem.get_y1()) * 2) / 16.0;
    this->velocidadeJogadorAtual = sqrt(pow((aceleracao_x*4.0), 2) + pow((aceleracao_y*4.0), 2));
    // cout <<"X "<< this->velocidadeJogadorAtual << endl;

    // this->jogador_x = x1;
    // this->jogador_y = y1;
};

void Arena::inputProcessing(char *input)
{
    char textAux[1000] = {""};
    strcpy(textAux, input);
    strcat(textAux, "config.xml");

    TiXmlDocument doc(textAux);

    bool loadOkay = doc.LoadFile();
    if (!loadOkay)
    {
        printf("Não foi posivel abrir o arquivo '%s'. Error='%s'.\n", textAux, doc.ErrorDesc());
        exit(1);
    }
    else
    {
        printf("'%s' - carregado com sucesso!\n", textAux);
    }

    TiXmlElement *rootElement = doc.RootElement();
    TiXmlElement *arena;
    TiXmlElement *jogador;
    TiXmlElement *inimigo;

    if (NULL != rootElement)
    {
        arena = rootElement->FirstChildElement("arquivoDaArena");
        jogador = rootElement->FirstChildElement("jogador");
        inimigo = rootElement->FirstChildElement("inimigo");
    }

    // info Jogador
    jogador->QueryFloatAttribute("vel", &(this->velocidadeJogadorBase));
    jogador->QueryFloatAttribute("velTiro", &(this->velocidadeTiroBase));

    //info Inimigo
    inimigo->QueryFloatAttribute("freqTiro", &(this->inimigo_freqTiro));
    inimigo->QueryFloatAttribute("vel", &(this->inimigo_vel));
    inimigo->QueryFloatAttribute("velTiro", &(this->inimigo_vel_tiro));

    //info Arena
    TiXmlElement *title = arena->FirstChildElement("nome");
    TiXmlElement *arq = arena->FirstChildElement("caminho");
    TiXmlElement *tipo = arena->FirstChildElement("tipo");

    this->nome = (strdup(title->GetText()));

    strcpy(textAux, arq->GetText());
    strcat(textAux, title->GetText());
    strcat(textAux, ".");
    strcat(textAux, tipo->GetText());

    // lendo Arena
    TiXmlDocument are(textAux);
    bool loadArenaOkay = are.LoadFile();
    if (!loadArenaOkay)
    {
        printf("Não foi posivel abrir o arquivo '%s'. Error='%s'.\n", textAux, are.ErrorDesc());
        exit(1);
    }
    else
    {
        printf("'%s' - carregado com sucesso!\n", textAux);
    }
    // ====================

    //Individuos!
    int cx, cy, raio, id;
    char *cor;

    TiXmlElement *rootSVG = are.RootElement();
    TiXmlElement *line = rootSVG->FirstChildElement("line");
    TiXmlElement *circle = rootSVG->FirstChildElement("circle");

    while (circle != 0)
    {
        circle->QueryIntAttribute("cx", &cx);
        circle->QueryIntAttribute("cy", &cy);
        circle->QueryIntAttribute("r", &raio);
        circle->QueryIntAttribute("id", &id);
        cor = strdup(circle->Attribute("fill"));

        if (!strcmp(cor, "blue"))
        {
            this->arena_config.set_raio(raio);
            this->arena_config.set_id(id);
            this->arena_config.set_x((float)cx);
            this->arena_config.set_y((float)cy);
            this->arena_config.set_corR(0);
            this->arena_config.set_corG(0);
            this->arena_config.set_corB(1);
        }
        else
        {
            if (!strcmp(cor, "red"))
            {
                add_individuos(cx, cy, raio, 1, 0, 0, id);
            }
            if (!strcmp(cor, "orange"))
            {
                add_individuos(cx, cy, raio, 1, 0.5, 0, id);
            }
            if (!strcmp(cor, "green"))
            {
                this->jogador_config.set_raio(raio);
                this->jogador_config.set_id(id);
                this->jogador_config.set_x((float)cx);
                this->jogador_config.set_y((float)cy);
                this->jogador_config.set_corR(0);
                this->jogador_config.set_corG(1);
                this->jogador_config.set_corB(0);
                this->raioOriginalJogador = raio;
            }
        }
        circle = circle->NextSiblingElement("circle");
    }
    // =====================================================

    //Pista de Decolagem!
    int x1, y1, x2, y2;
    float r, g, b;
    line->QueryIntAttribute("x1", &x1);
    line->QueryIntAttribute("x2", &x2);
    line->QueryIntAttribute("y1", &y1);
    line->QueryIntAttribute("y2", &y2);
    line->QueryIntAttribute("id", &id);
    cor = strdup(line->Attribute("style"));

    char *aux;
    int i = 0;
    aux = strtok(cor, "(,)");
    while (aux != NULL)
    {
        if (i == 1)
        {
            r = atof(aux);
        }
        if (i == 2)
        {
            g = atof(aux);
        }
        if (i == 3)
        {
            b = atof(aux);
        }
        i++;
        aux = strtok(NULL, "(,)");
    };

    set_partida(x1, y1, x2, y2, r, g, b, id);
    // =======================================

    float auxX = pista_decolagem.get_x2() - jogador_config.get_x(), auxY = pista_decolagem.get_y2() - jogador_config.get_y();
    this->direcao = anguloJogador(auxX, auxY);
    this->jogador_config.direcao = anguloJogador(auxX, auxY);
};

void Arena::print()
{
    //Nome
    cout << "Iniciando " << endl;
    cout << this->nome << endl;
    cout << " " << endl;

    //Config Arena
    cout << "Arena " << endl;
    cout << this->arena_config.get_raio() << endl;
    cout << this->arena_config.get_x() << endl;
    cout << this->arena_config.get_y() << endl;
    cout << " " << endl;

    //Config Jogador
    cout << "Jogador " << endl;
    this->jogador_config.print();
    cout << this->velocidadeJogadorBase << endl;
    cout << " " << endl;

    //Config Individuos
    cout << "Individuos " << endl;
    for (int i = 0; i < this->individuos.size(); i++)
    {
        this->individuos[i]->print();
    }
    cout << " " << endl;
};

void Arena::Desenha_Circulo(float raio, float r, float g, float b)
{
    GLfloat x, y;
    glColor3f(r, g, b);

    glBegin(GL_POLYGON);
        for (int ii = 0; ii < 360; ii++)
        {
            x = raio * cos(PI * ii / 180);
            y = raio * sin(PI * ii / 180);
            glVertex3f(x, y, 0); // cria o vértice
        }
    glEnd();
};

void Arena::Desenha_Individuos(vector<Circle *> *lista_individuos)
{
    float origemX = this->arena_config.get_x();
    float origemY = this->arena_config.get_y();
    Circle *ind;
    
    for (int i = 0; i < lista_individuos->size(); i++)
    {
        ind = this->individuos[i];
        glPushMatrix();
            glTranslatef(origemX - ind->get_x(), origemY - ind->get_y(), 0);
            if(ind->get_corG() == 0){
                // cout << ind->direcao << endl;
                Desenha_Jogador(1, 0, 0, ind->get_raio(), ind->thetaCanhao, ind->thetaHelice, ind->direcao);
            }
        glPopMatrix();
    }
};

void Arena::DesenhaBaseInimiga(vector<Circle *> *lista_individuos){
    float origemX = this->arena_config.get_x();
    float origemY = this->arena_config.get_y();
    Circle *ind;
    for (int i = 0; i < lista_individuos->size(); i++)
    {
        ind = this->individuos[i];
        glPushMatrix();
            glTranslatef(origemX - ind->get_x(), origemY - ind->get_y(), 0);
            if(ind->get_corG() != 0){
                glColor3f(ind->get_corR(), ind->get_corG(), ind->get_corB());
                
                glEnable(GL_TEXTURE_2D);
                glPushMatrix();
                        
                    gluQuadricNormals(baseInimiga, GLU_SMOOTH);
                    gluQuadricOrientation(baseInimiga, GLU_OUTSIDE);
                    gluQuadricTexture(baseInimiga, GL_TRUE);
                    
                    glBindTexture (GL_TEXTURE_2D, this->textureBaseInimiga);
                    gluSphere(baseInimiga,ind->get_raio(), 50, 50);
                glPopMatrix();
                glDisable(GL_TEXTURE_2D);
                
                // Desenha_Circulo(ind->get_raio(), ind->get_corR(), ind->get_corG(), ind->get_corB());
            
            }
            
        glPopMatrix();
    }
}

void Arena::Desenha_Retangulo(float height, float width, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
        glVertex3f(-width / 2, 0, 0.0);
        glVertex3f(width / 2, 0, 0.0);
        glVertex3f(width / 2, height, 0.0);
        glVertex3f(-width / 2, height, 0.0);
    glEnd();
};

void Arena::Desenha_Triangulo(float tam, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
        glVertex2f(0.00f, 0.00f);
        glVertex2f(0.25f * tam, -0.90f * tam);
        glVertex2f(-0.25f * tam, -0.90f * tam);
    glEnd();
};

void Arena::Desenha_Tiro()
{
    for (int i = 0; i < this->tiros.size(); i++)
    {
        float x = (this->arena_config.get_x() - this->tiros[i]->get_Inix());
        float y = (this->arena_config.get_y() - this->tiros[i]->get_Iniy());
        //cout << x << " " << y << endl;
        float rot = this->tiros[i]->get_direcao();
        //cout << rot << " rotação" << endl;
        float canhao = this->tiros[i]->get_canhao();
        //cout << canhao << " canhao" << endl;

        glPushMatrix();
            glTranslatef(x, y, 0);
            glRotatef(-rot, 0, 0, 1);
            

            if (this->tiros[i]->get_tipo() == 1)
            {
                glTranslatef(0, jogador_config.get_raio(), 0);
                glRotatef(canhao, 0, 0, 1);

                // glTranslatef(0, jogador_config.get_raio() / 4, 0);
            }

            this->tiros[i]->Desenha();
        glPopMatrix();
    }
};

void Arena::Desenha_Jogador(int ini, float x, float y, float raio, float thetaCanhao, float thetaHelice, float direcao)
{
    float rot;

    glPushMatrix();
        glTranslatef(x, y, 0);

        //========================================

        glRotatef(-direcao, 0, 0, 1);

        //========================================

        // Desenha_Circulo(raio,0.5,0.5,0.5);

        //desenha asa direita
        glPushMatrix();
            glTranslatef(raio / 3, -raio / 3, 0);
            //glScalef(0.1,0.3,1);
            glRotatef(-15.0, 0, 0, 1);
            Desenha_Retangulo(raio / 4, raio, 0, 0, 0);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(raio / 2 + raio / 10, -raio / 4, 0);
            Desenha_Retangulo(raio / 3, raio / 10, 0, 0, 0);
        glPopMatrix();

        //desenha asa esquerda
        glPushMatrix();
            glTranslatef(-raio / 3, -raio / 3, 0);
            //glScalef(0.1,0.3,1);
            glRotatef(15.0, 0, 0, 1);
            Desenha_Retangulo(raio / 4, raio, 0, 0, 0);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-raio / 2 - raio / 10, -raio / 4, 0);
            Desenha_Retangulo(raio / 3, raio / 10, 0, 0, 0);
        glPopMatrix();

        //canhão
        glPushMatrix();
            glTranslatef(0, raio, 0);
            glRotatef(thetaCanhao, 0, 0, 1);
            Desenha_Retangulo(raio / 4, raio / 10, 0, 0, 0);
        glPopMatrix();

        //desenha elipse
        glPushMatrix();
            glScalef(0.3, 1, 1);
            if(ini == 1){
                Desenha_Circulo(raio, 1, 0, 0);
            }else{
                Desenha_Circulo(raio, 0, 0.6, 0);
            }
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0, -raio, 0);
            Desenha_Retangulo(raio / 2, raio / 10, 0, 0, 0);
        glPopMatrix();

        //desenha cabine
        glPushMatrix();
            glTranslatef(0, raio / 2, 0);
            glScalef(0.1, 0.3, 1);
            Desenha_Circulo(raio, 0, 0, 0);
        glPopMatrix();

        //Helice Direita
        glPushMatrix();
            glTranslatef(-raio / 2 - raio / 10, raio / 15, 0);
            rot = 90.0;
            glRotatef(rot, 0, 0, 1);
            glPushMatrix();
                glRotatef(-thetaHelice, 1, 0, 0);
                Desenha_Triangulo(raio / 3.5, 0.8, 1, 0);
            glPopMatrix();
            glRotatef(-rot, 0, 0, 1);

            rot = -90.0;
            glRotatef(rot, 0, 0, 1);
            glPushMatrix();
                glRotatef(thetaHelice, 1, 0, 0);
                Desenha_Triangulo(raio / 3.5, 0.8, 1, 0);
            glPopMatrix();
        glPopMatrix();

        //Helice Esquerda
        glPushMatrix();
            glTranslatef(+raio / 2 + raio / 10, raio / 15, 0);
            rot = 90.0;
            glRotatef(rot, 0, 0, 1);
            glPushMatrix();
                glRotatef(thetaHelice, 1, 0, 0);
                Desenha_Triangulo(raio / 3.5, 0.8, 1, 0);
            glPopMatrix();
            glRotatef(-rot, 0, 0, 1);

            rot = -90.0;
            glRotatef(rot, 0, 0, 1);
            glPushMatrix();
                glRotatef(-thetaHelice, 1, 0, 0);
                Desenha_Triangulo(raio / 3.5, 0.8, 1, 0);
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
};

void Arena::Desenha_Pista(float x1, float x2, float y1, float y2, float corR, float corG, float corB)
{
    glPushMatrix();
        glColor3f(corR, corG, corB);
        glBegin(GL_LINES);
            glVertex3f(x1, y1, 0.0);
            glVertex3f(x2, y2, 0.0);
        glEnd();
    glPopMatrix();
};


void Arena::Desenha_Arena(Circle *arena, Circle *jogador, vector<Circle *> *lista_individuos, Line *pista)
{   
    glPushMatrix();

        glTranslatef(arena->get_x(), arena->get_y(), 0);
        // glTranslatef(arena->get_x(), arena->get_y(), -16*20);

        // Desenha_Circulo(arena->get_raio(), arena->get_corR(), arena->get_corG(), arena->get_corB());
        glColor3f(arena->get_corR(), arena->get_corG(), arena->get_corB());
        gluDisk(chao,0,arena->get_raio(),50,50);

        Desenha_Pista(arena->get_x() - pista->get_x1(), arena->get_x() - pista->get_x2(), arena->get_x() - pista->get_y1(), arena->get_x() - pista->get_y2(), pista->get_r(), pista->get_g(), pista->get_b());

        Desenha_Tiro();

        Desenha_Jogador(0,arena->get_x() - jogador->get_x(), arena->get_y() - jogador->get_y(), jogador->get_raio(), this->thetaCanhao, this->thetaHelice, this->direcao);

        Desenha_Individuos(lista_individuos);
        
        glPushMatrix();
            glTranslatef(0, 0, -16*20);//mudar
            
            glColor3f(0,0.5,0.5); //meu
            
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
                gluQuadricNormals(cilindro, GLU_SMOOTH);
                gluQuadricOrientation(cilindro, GLU_OUTSIDE);
                gluQuadricTexture(cilindro, GL_TRUE);
                
                glBindTexture (GL_TEXTURE_2D, textureParedes);
                gluCylinder(cilindro,arena->get_raio(),arena->get_raio(),16*20,50,50);//mudar meu
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);

            
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
                
                gluQuadricNormals(ceu, GLU_SMOOTH);
                gluQuadricOrientation(ceu, GLU_OUTSIDE);
                gluQuadricTexture(ceu, GL_TRUE);
                
                glBindTexture (GL_TEXTURE_2D, textureCeu);
                gluDisk(ceu,0,arena->get_raio(),50,50);
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
            
        glPopMatrix();

        DesenhaBaseInimiga(lista_individuos);

    glPopMatrix();
   
};

void Arena::Aceleracao()
{
    aceleracao_x = ((this->pista_decolagem.get_x2() - this->pista_decolagem.get_x1()) * 2) / 16.0;
    aceleracao_y = ((this->pista_decolagem.get_y2() - this->pista_decolagem.get_y1()) * 2) / 16.0;
    this->velocidadeJogadorAtual = sqrt(pow((aceleracao_x*4.0), 2) + pow((aceleracao_y*4.0), 2));
};

void Arena::inicioDecolagem()
{
    if (getDecolagem() == 0)
    {
        this->timeOld = glutGet(GLUT_ELAPSED_TIME);
        this->timeNew = this->timeOld;
        addEstadoDecolagem();
        this->deltaS = this->distanciaJogadorFimPista();
        float auxX = pista_decolagem.get_x2() - jogador_config.get_x(), auxY = pista_decolagem.get_y2() - jogador_config.get_y();
        this->direcao = anguloJogador(auxX, auxY);
    }
};

void Arena::colocaAviaoNosEixo(float x, float y, float direcao){

    jogador_config.set_x(x);
    jogador_config.set_y(y);
    this->direcao = direcao;
}

void Arena::decolando()
{

    float tempoAgr = ((this->timeNew - this->timeOld) / 1000.0);

    if (distanciaJogadorFimPista() < (this->deltaS / 2))
    {
        float m = ((2.0 * (this->deltaS / 2)) - distanciaJogadorFimPista()) / (this->deltaS / 2.0);
        float novoRaio = this->raioOriginalJogador * m;
        this->jogador_config.set_raio(novoRaio);
    }
    else
    {
        this->jogador_config.set_raio(this->raioOriginalJogador);
    }

    if (tempoAgr > 4.0)
    {
        addEstadoDecolagem();
        this->jogador_config.set_raio(2 * raioOriginalJogador);
        this->jogador_config.set_x(this->pista_decolagem.get_x2());
        this->jogador_config.set_y(this->pista_decolagem.get_y2());
        this->velocidadeJogadorAtual = sqrt(pow(this->velocidadeJogadorXAtual, 2) + pow(this->velocidadeJogadorYAtual, 2));
        // coutcout <<"J"<< this->velocidadeJogadorAtual << endl;
    }
    else
    {
        float auxX = pista_decolagem.get_x2() - jogador_config.get_x(), auxY = pista_decolagem.get_y2() - jogador_config.get_y();
        this->direcao = anguloJogador(auxX, auxY);

        timeNew = glutGet(GLUT_ELAPSED_TIME);

        this->velocidadeJogadorXAtual = (this->aceleracao_x * tempoAgr);
        this->deslAtualX = (this->velocidadeJogadorXAtual * tempoAgr) / 2;
        this->andaXjogador(this->deslAtualX - this->deslX);
        this->deslX = this->deslAtualX;

        this->velocidadeJogadorYAtual = (this->aceleracao_y * tempoAgr);
        this->deslAtualY = (this->velocidadeJogadorYAtual * tempoAgr) / 2;
        this->andaYjogador(this->deslAtualY - this->deslY);
        this->deslY = this->deslAtualY;
    }
};

float Arena::deslocX(float deltaT){
    return (((this->velocidadeJogadorAtual * 1.3*sqrt(2) * -multiplicadorDeslocamentoY(this->direcao)) * this->velocidadeJogadorBase) * deltaT) / 2;
}
float Arena::deslocY(float deltaT){
    return (((this->velocidadeJogadorAtual  * 1.3*sqrt(2) * -multiplicadorDeslocamentoX(this->direcao)) * this->velocidadeJogadorBase) * deltaT) / 2;
}

void Arena::decolou(float deltaT)
{
    this->timeNew = glutGet(GLUT_ELAPSED_TIME);
    this->deltaT = (this->timeNew - this->timeOld) / 1000.0;

    if (this->deltaT > 0)
    {
        this->timeOld = this->timeNew;
    }

    // float desl = (((this->velocidadeJogadorAtual * 1.3*sqrt(2) * -multiplicadorDeslocamentoY(this->direcao)) * this->velocidadeJogadorBase) * deltaT) / 2;

    // cout<<this->direcao<<endl;
    // float deslAux = desl * -multiplicadorDeslocamentoY(this->direcao);
    andaXjogador(this->deslocX( deltaT));
    if (this->encostandoNumInimigo() == 1)
    {
        andaXjogador(-this->deslocX( deltaT));
        this->addEstadoDecolagem();
    }

    // desl = (((this->velocidadeJogadorAtual  * 1.3*sqrt(2) * -multiplicadorDeslocamentoX(this->direcao)) * this->velocidadeJogadorBase) * deltaT) / 2;
    // deslAux = desl * -multiplicadorDeslocamentoX(this->direcao);
    andaYjogador(this->deslocY( deltaT));
    if (this->encostandoNumInimigo() == 1)
    {
        andaXjogador(-this->deslocX( deltaT));
        this->addEstadoDecolagem();
    }
    TrataForaDaArena(&jogador_config);
    
};

//Função encostar num inimigo
int Arena::encostandoNumInimigo()
{
    for (int i = 0; i < this->individuos.size(); i++)
    {
        Circle *aux = this->individuos[i];
        if (aux->get_corR() == 1 && aux->get_corG() == 0 && aux->get_corB() == 0)
        {
            float dist = sqrt(pow(aux->get_x() - this->jogador_config.get_x(), 2) + pow(aux->get_y() - this->jogador_config.get_y(), 2));
            if (dist <= (aux->get_raio() + this->jogador_config.get_raio()))
            {
                aux->colisao = 1;
                return 1;
            }
        }
    }
    return 0;
}

void Arena::trocaDeContexto()
{
    addEstadoDecolagem();
    this->timeOld = this->timeNew;
};

void Arena::andaXjogador(float x)
{
    this->jogador_config.set_x(this->jogador_config.get_x() + x);
    
};

void Arena::andaYjogador(float y)
{
    this->jogador_config.set_y(this->jogador_config.get_y() + y);
    
};

float Arena::anguloJogador(float x, float y)
{
    float hip = sqrt(pow(0 - x, 2) + pow(0 - y, 2));
    
    float angulo = asin(abs(x) / hip) * 180 / PI;
    
    if (x >= 0)
    {
        if (y >= 0)
        {
            return 180+angulo;
        }
        else
        {
            return -angulo;
        }
    }
    else
    {
        if (y >= 0)
        {
            return 90 + angulo;
        }
        else
        {
            return 90 - angulo;
        }
    }
};

float Arena::multiplicadorDeslocamentoX(float angulo)
{
    return cos(angulo * PI / 180.0) * this->velocidadeJogadorBase;
};

float Arena::multiplicadorDeslocamentoY(float angulo)
{
    return sin(angulo * PI / 180.0) * this->velocidadeJogadorBase;
};

void Arena::tiro(int tipo)
{
    float raio = jogador_config.get_raio();

    float xJ = jogador_config.get_x();
    float yJ = jogador_config.get_y();
    // cout <<xJ <<" " << yJ << endl;

    float xT = raio * sin(PI * this->direcao / 180);
    float yT = raio * cos(PI * this->direcao / 180);
    // cout <<-xT <<" " << -yT << endl;


    float yC = (raio / 4) * cos(PI * this->thetaCanhao / 180);
    float xC = (raio / 4) * sin(PI * this->thetaCanhao / 180);
    // cout <<-xC <<" " << -yC << endl;
    // cout <<xJ-xT-xC <<" " << yJ-yT-yC << endl;

    Bala *nova = new Bala;
    nova->set_tipo(tipo);
    nova->set_raio(jogador_config.get_raio());
    nova->set_direcao(this->direcao);
    nova->set_canhao(this->thetaCanhao);
    nova->set_velocidade(this->velocidadeJogadorAtual * this->velocidadeJogadorBase);
    nova->set_velocidadeAviao(this->velocidadeJogadorAtual);
    nova->set_velocidadeBase(this->velocidadeTiroBase);
    nova->set_multvelocidade(this->velocidadeJogadorBase);
    nova->set_Inix(xJ);
    nova->set_Iniy(yJ);
    nova->set_x(-xT);
    nova->set_y(-yT);

    this->tiros.push_back(nova);
};

void Arena::tiroInimigo(Circle* ind){
    
    if (ind->get_corG() == 0)
    {
        if(this->tempo_tiro_Inimi > (1/this->inimigo_freqTiro)){
            
            float raio = ind->get_raio();
            float xJ = ind->get_x();
            float yJ = ind->get_y();

            float xT = raio * sin(PI * ind->direcao / 180);
            float yT = raio * cos(PI * ind->direcao / 180);

            float yC = (raio / 4) * cos(PI * ind->thetaCanhao / 180);
            float xC = (raio / 4) * sin(PI * ind->thetaCanhao / 180);

            Bala *nova = new Bala;
            nova->set_tipo(1);
            nova->balaIni = 1;
            nova->set_raio(ind->get_raio());
            nova->set_direcao(ind->direcao);
            nova->set_canhao(ind->thetaCanhao);

            nova->set_velocidade(this->velocidadeJogadorAtual * ind->inimigo_vel);
            nova->set_velocidadeAviao(this->velocidadeJogadorAtual);
            nova->set_velocidadeBase(ind->inimigo_vel_tiro);
            nova->set_multvelocidade(ind->inimigo_vel);
            nova->set_Inix(xJ);
            nova->set_Iniy(yJ);
            nova->set_x(-xT);
            nova->set_y(-yT);

            this->tiros.push_back(nova);
        }
    }
    
};

void Arena::limpaTiros()
{

    for (int i = 0; i < this->tiros.size(); i++)
    {
        if(this->tiros[i]->get_tipo() == 1){
            float xx = this->tiros[i]->get_Inix(), yy = this->tiros[i]->get_Iniy();
            if ((xx < this->ortho_Config(1, -1)) || (xx > this->ortho_Config(1, 1)) || (yy < this->ortho_Config(2, -1)) || (yy > this->ortho_Config(2, 1)))
            {
                this->tiros.erase(this->tiros.begin() + i);
            }
        }else{
            if(!(this->tiros[i]->get_time() < 2)){
                
                for (int j = 0; j < this->individuos.size(); j++)
                {
                    Circle *aux = this->individuos[j];
                    if (aux->get_corG() != 0)
                    {
                        float dist = sqrt(pow(aux->get_x() - this->tiros[i]->get_Inix(), 2) + pow(aux->get_y() - this->tiros[i]->get_Iniy(), 2));
                        
                        if (dist <= (aux->get_raio() + (this->tiros[i]->get_raio()*0.3)))
                        {
                            aux->colisao = 1;
                            this->individuos.erase(this->individuos.begin() + j);
                            break;
                        }
                    }
                }
            
                this->tiros.erase(this->tiros.begin() + i);
            }
        }
        
    }
};

void Arena::atualizaTiros(float p)
{
    this->limpaTiros();

    for (int i = 0; i < this->tiros.size(); i++)
    {
        Bala* tiro = this->tiros[i];

        // if(test == 0){
            tiro->Atualiza(p);
        // }

        int test = 0;
        if(tiro->balaIni == 0 && tiro->get_tipo() == 1){
            for (int j = 0; j < this->individuos.size(); j++)
            {
                Circle *aux = this->individuos[j];
                if (aux->get_corG() == 0)
                {
                    float dist = sqrt(pow(aux->get_x() - (tiro->get_Inix() + tiro->get_x()), 2) + pow(aux->get_y() - (tiro->get_Iniy() + tiro->get_y()), 2));
                    
                    // cout << tiro->get_raio()/15 << endl;
                    if (dist <= (aux->get_raio() + (tiro->get_raio()/15)))
                    {   

                        aux->colisao = 1;
                        test = 1;
                        this->tiros.erase(this->tiros.begin() + i);
                        // break;
                    }
                }
            }
        }

        if(tiro->balaIni == 1){

            float dist = sqrt(pow(this->jogador_config.get_x() - (tiro->get_Inix() + tiro->get_x()), 2) + pow(this->jogador_config.get_y() - (tiro->get_Iniy() + tiro->get_y()), 2));
            
            if (dist <= (this->jogador_config.get_raio() + (tiro->get_raio()/15)))
            {
                if(this->getDecolagem() == 3){
                    
                    this->addEstadoDecolagem();
                    test = 1;
                    this->tiros.erase(this->tiros.begin() + i);
                    // break;
                }
            }
        }

        
    }
};

int Arena::saindoPontaArena(Circle* p){
    float dist = sqrt(pow(this->arena_config.get_x() - p->get_x(),2) + pow(this->arena_config.get_y() - p->get_y(),2));
    if( dist < (this->arena_config.get_raio()-p->get_raio())){
        return 0;
    }else{
        return 1;
        // cout << "bateu" << endl;
    }
}

int Arena::saindoMeioArena(Circle* p){
    float dist = sqrt(pow(this->arena_config.get_x() - p->get_x(),2) + pow(this->arena_config.get_y() - p->get_y(),2));
    if( dist < (this->arena_config.get_raio())){
        return 0;
    }else{
        return 1;
        // cout << "bateu" << endl;
    }
}

void Arena::TrataForaDaArena(Circle* p){
    float m, raio, aa, bb, cc, a, b, c, delta, y1, y2, x1, x2, distP1, distP2;

    if(saindoMeioArena(p) == 1){
        this->flag = 0;

        m = (p->get_y() - p->yl) / (p->get_x() - p->xl);
        bb = this->arena_config.get_y(); // (x-aa)^2 + (y-bb)^2 = r^2
        aa = this->arena_config.get_x(); // (x-aa)^2 + (y-bb)^2 = r^2
        cc = (p->get_x()) - aa - (p->get_y()/m);
        raio = this->arena_config.get_raio() - 5; // não fica em cima da circunferencia
        // cout << raio << endl;

        b = ((2*cc)/m) - 2*bb;
        a = (1/pow(m,2)) + 1;
        c = pow(cc,2) + pow(bb,2) - pow(raio,2);
        delta = pow(b,2) - (4*a*c);

        if(delta > 0){
            // cout << "definindo" << endl;
            y1 = (-b + sqrt(delta)) / (2*a);
            y2 = (-b - sqrt(delta)) / (2*a);
            x1 = (y1 - p->get_y())/m + p->get_x();
            x2 = (y2 - p->get_y())/m + p->get_x();
            distP1 = sqrt(pow(x1 - p->get_x(),2) + pow(y1 - p->get_y(),2));
            distP2 = sqrt(pow(x2 - p->get_x(),2) + pow(y2 - p->get_y(),2));

            if(distP1 > distP2){
                p->set_x(x1);
                p->set_y(y1);
            }else{
                p->set_x(x2);
                p->set_y(y2);
            }
        }else{
            if(p->get_corB() == 1){ 
                andaXjogador(-0.1); 
                andaYjogador(-0.1); 
                this->curvaAviao(1); 
            }else{
                p->andaXCircle(-0.1);
                p->andaYCircle(-0.1);
                p->direcao += 0.5 * p->inimigo_vel;
            }
            
        }

    }else{
        if(saindoPontaArena(p) == 1){
            p->yl = p->get_y();
            p->xl = p->get_x();
            this->flag = 1;
        }
        
    }
}

void Arena::atualizaInimigos(float p){
    Circle *ind;
    srand(time(NULL));
    this->tempo_tiro_Inimi += p;

    for (int i = 0; i < this->individuos.size(); i++)
    {   
        int decidir[] = {2,0,1,4,3,0,2,4,1,0,1,3,4,2,0,3};
        ind = this->individuos[i];
        if(ind->isInimigoVoador == 1 && ind->colisao == 0){
            
            if(decidir[rand()%15] > 0){
                int estado = decidir[rand()%9];
                if( estado == 1){
                    ind->direcao += 0.5 * ind->inimigo_vel;
                }

                if(estado == 2){
                    ind->direcao -= 0.5 * ind->inimigo_vel;
                }

                if(estado == 3){
                    //deslocamento vertical subir 
                }
                
                if(estado == 4){
                    //deslocamento vertical descer
                }
            }

            ind->inimigo_vel = this->inimigo_vel;
            
            float desl = (((this->velocidadeJogadorAtual * sqrt(2) * -multiplicadorDeslocamentoY(ind->direcao)) * ind->inimigo_vel) * p) / 2;
            ind->andaXCircle(desl);
            
            desl = (((this->velocidadeJogadorAtual  * sqrt(2) * -multiplicadorDeslocamentoX(ind->direcao)) * ind->inimigo_vel) * p) / 2;
            ind->andaYCircle(desl);
            
            ind->thetaHelice += ((70 + ind->inimigo_vel) * 60 / 360.0) + ((120) * 60 / 360.0);

            tiroInimigo(ind);

            TrataForaDaArena(ind);
        }

        if(ind->colisao == 1){
            this->individuos.erase(this->individuos.begin() + i);
        }
        
    }
    
    if(this->tempo_tiro_Inimi > (1/this->inimigo_freqTiro)) this->tempo_tiro_Inimi = 0;

}