#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Arena.h"

#define PI 3.1415

using namespace std;
Arena arena_modelo, arena_reset;

// ------- Variaveis Globais ------
float timeOld = 0;
float deltaX, deltaY, deltaXold = 0, deltaYold = 0;
float letras[256] = {0};
int mouseAt = 0;
int basesIni = 0;
int basesIniAgr = 0;
int eixo = 0;
char textImprimir[100];
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

double camXYAngle=0;
double camXZAngle=0;
int buttonDown = 0;



// --------------------------------

void limpaTexto(){
    int i = 0; 
    while(i < 100){
        textImprimir[i++] = '\0';
    }
}

void RasterChars(float x, float y, float z, char * text, double r, double g, double b)
{
    //Push to recover original attributes
    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        //Draw text in the x, y, z position
        glColor3f(r,g,b);
        glRasterPos3f(x, y, z);
        const char* tmpStr;
        tmpStr = text;
        while( *text ){
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *text++);
            // tmpStr++;
        }
    glPopAttrib();
}

void PrintText(float x, float y,char * text, double r, double g, double b)
{
    //Draw text considering a 2D space (disable all 3d features)
    glMatrixMode (GL_PROJECTION);
    //Push to recover original PROJECTION MATRIX
    glPushMatrix();
        glLoadIdentity ();
        glOrtho(arena_modelo.ortho_Config(1, 1), arena_modelo.ortho_Config(1, -1), arena_modelo.ortho_Config(2, -1), arena_modelo.ortho_Config(2, 1), -1.0, 1.0);
        RasterChars(x, y, 0, text, r, g, b);    
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
}

// Desenha um texto na janela GLUT
void DesenhaTexto(float x, float y, float z,char *string) 
{  
  	glPushMatrix();
        // Posição no universo onde o texto será colocado          
        glRasterPos3f(x,y,z);
        glColor3f(0,0,0); 
        // Exibe caracter a caracter
        while(*string)
             glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*string++); 
	glPopMatrix();
}

//

void normalize(float a[3]){
    double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); 
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

void cross(float a[3], float b[3], float c[3]){
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = a[2]*b[0] - a[0]*b[2];
    c[2] = a[0]*b[1] - a[1]*b[0];
}

void MygluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,GLdouble upz){
    float forward[3], side[3], up[3];
    //column-major order
    GLfloat m[4][4] = { 1,0,0,0,
                        0,1,0,0,
                        0,0,1,0,
                        0,0,0,1};

    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    normalize(forward);

    /* Side = forward x up */
    cross(forward, up, side);
    normalize(side);

    /* Recompute up as: up = side x forward */
    cross(side, forward, up);
    //column-major
    //C  L
    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];
    //C  L
    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];
    //C  L
    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];

    glMultMatrixf(&m[0][0]);
    glTranslated(-eyex, -eyey, -eyez);
}

void ChangeCoordSys(GLdouble ax, GLdouble ay, GLdouble az, GLdouble bx, GLdouble by, GLdouble bz, GLdouble cx, GLdouble cy, GLdouble cz){
    float z[3], side[3], up[3];
    GLfloat m[4][4] = { 1,0,0,0,
                        0,1,0,0,
                        0,0,1,0,
                        0,0,0,1};

    z[0] = (ax - bx);
    z[1] = (ay - by);
    z[2] = (az - bz);

    up[0] = (cx - bx);
    up[1] = (cy - by);
    up[2] = (cz - bz);

    normalize(z);

    cross(up, z, side);
    normalize(side);

    /* Recompute up as: up = side x forward */
    cross(z, side, up);

    m[0][0] = side[0];
    m[0][1] = side[1];
    m[0][2] = side[2];

    m[1][0] = up[0];
    m[1][1] = up[1];
    m[1][2] = up[2];

    m[2][0] = z[0];
    m[2][1] = z[1];
    m[2][2] = z[2];

    m[3][0] = bx;
    m[3][1] = by;
    m[3][2] = bz;
    
    glMultMatrixf(&m[0][0]);
}



char inttochar(int val, int base){
    return digits[val % base];
}

//
int zCam = -300, yCam = 500, xCam = 500;
int camera = 1;
int angCam = 0;

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    Circle jogadorT = arena_modelo.get_jogador();
    glMatrixMode(GL_PROJECTION);


        float x2 = jogadorT.get_x() - 500 - sin(jogadorT.direcao * PI /180) * (jogadorT.get_raio()/3);
        float y2 = jogadorT.get_y() - 500 - cos(jogadorT.direcao * PI /180) * (jogadorT.get_raio()/3);

        float lookX = jogadorT.get_x() - 500 - 20*arena_modelo.multiplicadorDeslocamentoY(jogadorT.direcao);
        float lookY = jogadorT.get_y() - 500 - 20*arena_modelo.multiplicadorDeslocamentoX(jogadorT.direcao);
        
        float xC = jogadorT.get_x() - 500 - sin(jogadorT.direcao * PI /180) * jogadorT.get_raio();
        float yC = jogadorT.get_y() - 500 - cos(jogadorT.direcao * PI /180) * jogadorT.get_raio();

        float lookXC = jogadorT.get_x() - 500 - (jogadorT.get_raio()/4)*sin(-jogadorT.thetaCanhao * PI /180) ;
        float lookYC = jogadorT.get_y() - 500 - (jogadorT.get_raio()/4)*cos(-jogadorT.thetaCanhao * PI /180) ;
        glLoadIdentity();
        if(camera == 3){
            gluPerspective(90, 1, 3, 1000);
            // glTranslatef(jogadorT.get_x(),jogadorT.get_y(),0);
            // glRotatef(180,0,0,0);
            // gluLookAt(xCam, yCam, zCam, 500, 500, 1, 0, 1, 0);
            // gluLookAt(500-lookX, 500-lookY, jogadorT.get_z()-10, 500-x2, 500-y2, jogadorT.get_z(), 0, 0, -1);
            glRotatef(180,1,0,0);
            glRotatef(180,0,0,1);


            glTranslatef(0,0,30);
            glRotatef(camXZAngle,1,0,0);
            glRotatef(camXYAngle,0,0,1);
            
            glRotatef(jogadorT.direcao,0,0,1);
            glTranslatef(-500+(jogadorT.get_x() - 500),-500+(jogadorT.get_y() - 500),-jogadorT.get_z());
            
            // glRotatef(180,0,1,0);         
        }

        if(camera == 1){
            gluPerspective(90, 1, 1, 1000);
            gluLookAt(500 - x2, 500 - y2, (jogadorT.get_z()-10) - 7.5*sin(jogadorT.direcaoZ * PI /180), (500 - lookX), (500 - lookY), (jogadorT.get_z()-10) + 7.5*sin(jogadorT.direcaoZ * PI /180), 0, 0, -1);    
            
        }

        if(camera == 2){
            gluPerspective(90, 1, 1, 1000);
            gluLookAt(500 - xC, 500 - yC, (jogadorT.get_z()-2) - sin(jogadorT.direcaoZ * PI /180) - sin(jogadorT.thetaCanhaoZ * PI /180), (500 + lookXC), (500 + lookYC), (jogadorT.get_z()-2) + sin(jogadorT.direcaoZ * PI /180) + (jogadorT.get_raio()/4)*sin(jogadorT.thetaCanhaoZ * PI /180) , 0, 0, -1);
        }

    glMatrixMode(GL_MODELVIEW);
    
    GLfloat light_position[] = { 500.0, 500.0, -20*jogadorT.get_raio(), 1 };
    GLfloat light0[]={1,1,1,1};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,light0);
    
    // GLfloat farol_position[] = { jogadorT.get_x() - sin(jogadorT.direcao * PI /180) * (jogadorT.get_raio()), jogadorT.get_y() - cos(jogadorT.direcao * PI /180) * (jogadorT.get_raio()), jogadorT.get_z(), 1 };
    // GLfloat farol_direction[] = {jogadorT.get_x() - 20*arena_modelo.multiplicadorDeslocamentoY(jogadorT.direcao), jogadorT.get_y() - 20*arena_modelo.multiplicadorDeslocamentoX(jogadorT.direcao), -(jogadorT.get_z() + 20*arena_modelo.multiplicadorDeslocamentoY(jogadorT.direcaoZ))};
    // GLfloat white[4] = { 1, 1, 1, 0 };
    // glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 1);
    // glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, farol_direction);
    // glLightfv(GL_LIGHT1, GL_POSITION, farol_position);
    // glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 32);
    // glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.2);
    // glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0);
    // glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0);
    // glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    // glLightfv(GL_LIGHT1, GL_SPECULAR, white);

    arena_modelo.Desenha();

    if(basesIniAgr != 0 && arena_modelo.getDecolagem() > 3){
        char perdeu[] = "Game Over!!! Aperte 'r' para recomecar!";
        PrintText(700, 500, perdeu , 1,0,0);
        // strcpy(textImprimir, "Game Over!!! Aperte 'r' para recomecar!");
        // glPushMatrix();
            
        //     DesenhaTexto(750,500,-1,textImprimir);
        // glPopMatrix();
    }

    if(basesIniAgr == 0){
        char ganhou[] = "Parabens!!! Voce Venceu. Aperte 'r' para recomecar!";
        PrintText(750, 500, ganhou, 0,1,0);
        // strcpy(textImprimir, "Parabens!!! Voce Venceu. Aperte 'r' para recomecar!");
        // glPushMatrix();
            
        //     DesenhaTexto(750,500,-1,textImprimir);
        // glPopMatrix();
    }
    
    // glMatrixMode(GL_PROJECTION);
    limpaTexto();
    strcpy(textImprimir, "Bases Inimigas: ");
    char c = (basesIni-basesIniAgr) + '0';
    textImprimir[strlen(textImprimir)] = c ;
    
    PrintText(500-50, 500+280, textImprimir, 1,1,1);

    limpaTexto();
    strcpy(textImprimir, "Restam ");
    char cc = basesIniAgr + '0';
    textImprimir[strlen(textImprimir)] = cc ;
    textImprimir[strlen(textImprimir)+1] = '\0';
    const char aux[] = " bases inimigas";
    strcat(textImprimir, aux);
    // strcat(textImprimir, &a);
    PrintText(500-50, 500+260, textImprimir, 1,1,1);
    // glMatrixMode(GL_MODELVIEW);
    
    
    //glFlush();
    glutSwapBuffers(); 
}

void init(){

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_DEPTH_TEST);
    // glDepthMask(GL_FALSE);
    // glDepthFunc(GL_LESS); 

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    // glShadeModel (GL_FLAT);
    glShadeModel (GL_SMOOTH);

    // glOrtho(arena_modelo.ortho_Config(1, 1), arena_modelo.ortho_Config(1, -1), arena_modelo.ortho_Config(2, -1), arena_modelo.ortho_Config(2, 1), -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int lastX = 0;
int lastY = 0;

void motion(int x, int y){
    if (deltaXold == 0)
    {
        deltaXold = x;
    }
    if (deltaYold == 0)
    {
        deltaYold = y;
    }
    if (x - deltaXold > 0)
    {
        arena_modelo.miraCanhao(1);
        deltaXold = x;
        //cout << "entrei" << endl;
    }
    if (x - deltaXold < 0)
    {
        arena_modelo.miraCanhao(-1);
        deltaXold = x;
    }
    
    if (y - deltaYold > 0)
    {
        arena_modelo.miraCanhaoZ(1);
        deltaYold = y;
        //cout << "entrei" << endl;
    }
    if (y - deltaYold < 0)
    {
        arena_modelo.miraCanhaoZ(-1);
        deltaYold = y;
    }
}

void mouseAtivMotion(int x, int y){
    if(camera == 3){
        if (buttonDown == 0) return;
        
        camXYAngle += x - lastX;
        camXZAngle += y - lastY;
        
        camXYAngle = (int)camXYAngle % 360;
        camXZAngle = (int)camXZAngle % 360;
        
        if(camXYAngle >= 180){
            camXYAngle = 180;
        }

        if(camXYAngle <= -180){
            camXYAngle = -180;
        }

        if(camXZAngle >= 60){
            camXZAngle = 60;
        }

        if(camXZAngle <= -60){
            camXZAngle = -60;
        }
        
        lastX = x;
        lastY = y;
        // printf("oi\n");
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y){

    deltaX = x;
    deltaY = y;
    if (arena_modelo.getDecolagem() == 3){
        if (GLUT_LEFT_BUTTON == button)
        {
            if (GLUT_UP == state)
            {
                mouseAt = 1;
            }
            if (mouseAt == 1)
            {
                //cout << "entrei1" << endl;
                arena_modelo.tiro(1);
                mouseAt = 0;
            }
        }
        if (GLUT_MIDDLE_BUTTON == button)
        {
            // cout << "entrei2" << endl;
        }
        if (GLUT_RIGHT_BUTTON == button)
        {
            if (GLUT_UP == state)
            {
                mouseAt = 1;
            }
            if (mouseAt == 1)
            {
                //cout << "entrei1" << endl;
                arena_modelo.tiro(2); // modificar tamanho
                mouseAt = 0;
            }
        }
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        lastX = x;
        lastY = y;
        buttonDown = 1;
    } 
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        buttonDown = 0;
    }
    glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y){
    switch (key)
    {
    case '3':
        letras['3'] = 1;
        break;
    case '1':
        letras['1'] = 1;
        break;
    case '2':
        letras['2'] = 1;
        break;
    case 'a':
        letras['a'] = 1;
        break;
    case 'd':
        letras['d'] = 1;
        break;
    case 'w':
        letras['w'] = 1;
        break;
    case 's':
        letras['s'] = 1;
        break;
    case 'u':
        letras['u'] = 1;
        break;
    case 'r':
        letras['r'] = 1;
        break;
    case '+':
        letras['+'] = 1;
        break;
    case '-':
        letras['-'] = 1;
        break;
    default:
        break;
    }
}

void keyup(unsigned char key, int x, int y){
    switch (key)
    {
    case '3':
        letras['3'] = 0;
        break;
    case '1':
        letras['1'] = 0;
        break;
    case '2':
        letras['2'] = 0;
        break;
    case 'a':
        letras['a'] = 0;
        break;
    case 'd':
        letras['d'] = 0;
        break;
    case 'w':
        letras['w'] = 0;
        break;
    case 's':
        letras['s'] = 0;
        break;
    case 'u':
        letras['u'] = 0;
        break;
    case 'r':
        letras['r'] = 0;
        break;
    case '+':
        letras['+'] = 0;
        break;
    case '-':
        letras['-'] = 0;
        break;
    default:
        break;
    }
}

void idle(void){   
    float timeNew = glutGet(GLUT_ELAPSED_TIME);
    float deltaT = (timeNew - timeOld) / 1000.0;

    if (deltaT > 0)
    {
        timeOld = timeNew;
    }else{
        return;
    }
    
    if(eixo == 0){
        arena_modelo.colocaAviaoNosEixo(arena_reset.get_jogador().get_x(), arena_reset.get_jogador().get_y(), arena_reset.get_jogador().direcao);
    }

// ---------- Tratando Casos --------------

    if (letras['1'] == 1)
    {
        camera = 1;
    }

    if (letras['2'] == 1)
    {
        camera = 2;
    }

    if (letras['3'] == 1)
    {
        camera = 3;
    }

    if (letras['r'] == 1) // reset
    {
        eixo = 0;
        arena_modelo.reset(arena_reset);
    }

    if (letras['u'] == 1) // decola
    {
        eixo = 1;
        arena_modelo.inicioDecolagem();
    }

    if (arena_modelo.getDecolagem() == 1)
    {
        arena_modelo.decolando();
    }
    else
    {
        if (arena_modelo.getDecolagem() == 2)
        {
            arena_modelo.trocaDeContexto();
        }

        if (arena_modelo.getDecolagem() == 3)
        {

            if (letras['a'] == 1)
            {
                arena_modelo.curvaAviao(0.9);
            }

            if (letras['d'] == 1)
            {
                arena_modelo.curvaAviao(-0.9);
            }

            if (letras['s'] == 1)
            {
                arena_modelo.aviaoZ(0.9);
            }

            if (letras['w'] == 1)
            {
                arena_modelo.aviaoZ(-0.9);
            }

            if(letras['w'] == 0 && letras['s'] == 0){

                if(arena_modelo.get_jogador().direcaoZ >= 0){
                    arena_modelo.aviaoZ(-0.5);
                }
                if(arena_modelo.get_jogador().direcaoZ <= 0){
                    arena_modelo.aviaoZ(0.5);
                }
            }

            if (letras['+'] == 1)
            {
                arena_modelo.machaVelocidade(0.05);
            }

            if (letras['-'] == 1)
            {
                arena_modelo.machaVelocidade(-0.05);
            }
            arena_modelo.decolou(deltaT);
        }
    }

    arena_modelo.atualizaTiros(deltaT);
    arena_modelo.giraHelice();
    basesIniAgr = arena_modelo.BasesInimigas();
    arena_modelo.atualizaInimigos(deltaT);
    glutPostRedisplay();
}

int main(int argc, char **argv){
    //Carregando Dados

    arena_modelo.inputProcessing(argv[1]);
    arena_reset.inputProcessing(argv[1]);
    //arena_modelo.print();
    basesIni = arena_modelo.BasesInimigas();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("trabalho 4");
    init();
    
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_BLEND);
    // glDepthMask(true);
    arena_modelo.textureParedes = arena_modelo.LoadTextureRAW("wxp.bmp");
    arena_modelo.textureBaseInimiga = arena_modelo.LoadTextureRAW("largada.bmp");
    arena_modelo.textureBala = arena_modelo.LoadTextureRAW("largada.bmp");
    arena_modelo.textureChao = arena_modelo.LoadTextureRAW("grass.bmp");
    arena_modelo.textureAviao = arena_modelo.LoadTextureRAW("camuflado.bmp");
    arena_modelo.textureAviaoIni = arena_modelo.LoadTextureRAW("largada.bmp");
    arena_modelo.textureCeu = arena_modelo.LoadTextureRAW("grass.bmp");


    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyup);
    glutPassiveMotionFunc(motion);
    glutMotionFunc(mouseAtivMotion);
    glutMouseFunc(mouse);

    glutIdleFunc(idle);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}