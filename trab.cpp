#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Arena.h"

using namespace std;
Arena arena_modelo = Arena(), arena_reset = Arena();

// ------- Variaveis Globais ------
float timeOld = 0;
float deltaX, deltaY, deltaXold = 0, deltaYold = 0;
float letras[256] = {0};
int mouseAt = 0;
int basesIni = 0;
int basesIniAgr = 0;
int eixo = 0;
char textImprimir[1000];



// --------------------------------


void RasterChars(float x, float y, float z, const char * text, double r, double g, double b)
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
        while( *tmpStr ){
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
            tmpStr++;
        }
    glPopAttrib();
}

void PrintText(float x, float y, const char * text, double r, double g, double b)
{
    //Draw text considering a 2D space (disable all 3d features)
    glMatrixMode (GL_PROJECTION);
    //Push to recover original PROJECTION MATRIX
    glPushMatrix();
        glLoadIdentity ();
        glOrtho(arena_modelo.ortho_Config(1, 1), arena_modelo.ortho_Config(1, -1), arena_modelo.ortho_Config(2, -1), arena_modelo.ortho_Config(2, 1), -1.0, 1.0);
        RasterChars(x, y, 0, text, r, g, b);    
    glPopMatrix();
    // glMatrixMode (GL_MODELVIEW);
}

// Desenha um texto na janela GLUT
void DesenhaTexto(float x, float y, float z,char *string) 
{  
  	glPushMatrix();
        // Posição no universo onde o texto será colocado          
        glRasterPos3f(x,y,z); 
        // Exibe caracter a caracter
        while(*string)
             glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*string++); 
	glPopMatrix();
}

//

void normalize(float a[3])
{
    double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); 
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}
void cross(float a[3], float b[3], float c[3])
{
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = a[2]*b[0] - a[0]*b[2];
    c[2] = a[0]*b[1] - a[1]*b[0];
}

void MygluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
      GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
      GLdouble upz)
{
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

void ChangeCoordSys(
        GLdouble ax, GLdouble ay, GLdouble az, 
        GLdouble bx, GLdouble by, GLdouble bz, 
        GLdouble cx, GLdouble cy, GLdouble cz)
{
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


//
int zCam = -300, yCam = 500, xCam = 500;
int camA = 0;
int angCam = 0;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   

    glMatrixMode(GL_PROJECTION);

        glLoadIdentity();
        if(camA == 0){
            gluPerspective(90, 1, 3, 1000);
            gluLookAt(xCam, yCam, zCam, 500, 500, 1, 0, 1, 0);

            Circle jogadorT = arena_modelo.get_jogador();
        }else{
            gluPerspective(90, 1, 1, 700);
            Circle jogadorT = arena_modelo.get_jogador();
            float x2 = jogadorT.get_x() - 500;
            float y2 = jogadorT.get_y() - 500;
            float x1 = jogadorT.get_x() - 500 + 5*arena_modelo.multiplicadorDeslocamentoY(jogadorT.direcao);
            float y1 = jogadorT.get_y() - 500 + 5*arena_modelo.multiplicadorDeslocamentoX(jogadorT.direcao);
            
            gluLookAt(500 - x1, 500 - y1, jogadorT.get_z()-30, 500 - x2, 500 - y2, jogadorT.get_z()-25, 0, 0, -1);    
        }

    glMatrixMode(GL_MODELVIEW);
    
    GLfloat light_position[] = { 500.0, 500.0, -250.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);


    arena_modelo.Desenha();

    if(basesIniAgr != 0 && arena_modelo.getDecolagem() > 3){
        PrintText(700, 500, "Game Over!!! Aperte 'r' para recomecar!", 1,0,0);
        // strcpy(textImprimir, "Game Over!!! Aperte 'r' para recomecar!");
        // glPushMatrix();
            
        //     DesenhaTexto(750,500,-1,textImprimir);
        // glPopMatrix();
    }

    if(basesIniAgr == 0){
        PrintText(750, 500, "Parabens!!! Voce Venceu. Aperte 'r' para recomecar!", 0,1,0);
        // strcpy(textImprimir, "Parabens!!! Voce Venceu. Aperte 'r' para recomecar!");
        // glPushMatrix();
            
        //     DesenhaTexto(750,500,-1,textImprimir);
        // glPopMatrix();
    }
    

    strcpy(textImprimir, "Bases Inimigas: ");
    const char c = (basesIni-basesIniAgr) + '0';
    strcat(textImprimir, &c);
    PrintText(500-50, 500+280, textImprimir, 1,1,1);

    strcpy(textImprimir, "Restam ");
    const char cc = basesIniAgr + '0';
    strcat(textImprimir, &cc);
    strcat(textImprimir, " bases inimigas");
    PrintText(500-50, 500+260, textImprimir, 1,1,1);
    
    
    //glFlush();
    glutSwapBuffers(); 
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    // glDepthRange(0,700);
    // glDepthMask(GLU_TRUE);
    // glDepthFunc(GL_LESS);

    // glEnable(GL_DEPTH_TEST);
    // glDepthMask(GL_FALSE);
    // glDepthFunc(GL_LESS); 

    // glEnable( GL_TEXTURE_2D );
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    // glShadeModel (GL_FLAT);
    glShadeModel (GL_SMOOTH);

    // glOrtho(arena_modelo.ortho_Config(1, 1), arena_modelo.ortho_Config(1, -1), arena_modelo.ortho_Config(2, -1), arena_modelo.ortho_Config(2, 1), -1.0, 1.0);
}

void motion(int x, int y)
{
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
        arena_modelo.miraCanhao(3);
        deltaXold = x;
        //cout << "entrei" << endl;
    }
    if (x - deltaXold < 0)
    {
        arena_modelo.miraCanhao(-3);
        deltaXold = x;
    }
}

void mouse(int button, int state, int x, int y)
{

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
}

void keyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '0':
        letras['0'] = 1;
        camA = 0;
        break;
    case '1':
        letras['1'] = 1;
        camA = 1;
        break;
    case '8':
        letras['8'] = 1;
        yCam += 5;
        break;
    case '2':
        letras['2'] = 1;
        yCam -= 5;
        break;
    case '4':
        letras['4'] = 1;
        xCam -= 5;
        break;
    case '6':
        letras['6'] = 1;
        xCam += 5;
        break;
    case '5':
        letras['5'] = 1;
        zCam -= 5;
        break;
    case '9':
        letras['9'] = 1;
        zCam += 5;
        break;
    case '7':
        letras['7'] = 1;
        zCam = -300;
        xCam = 500;
        yCam = 500;
        angCam = 0;
        break;
    case 'o':
        letras['o'] = 1;
        angCam += 5;
        break;
    case 'p':
        letras['p'] = 1;
        angCam -= 5;
        break;
    case 'a':
        letras['a'] = 1;
        break;
    case 'd':
        letras['d'] = 1;
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

void keyup(unsigned char key, int x, int y)
{
    switch (key)
    {
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

void idle(void)
{   
    float timeNew = glutGet(GLUT_ELAPSED_TIME);
    float deltaT = (timeNew - timeOld) / 1000.0;


    if (deltaT > 0)
    {
        timeOld = timeNew;
    }else{
        return;
    }

    if (letras['r'] == 1)
    {
        eixo = 0;
        arena_modelo.reset(arena_reset);
    }

    if(eixo == 0){
        arena_modelo.colocaAviaoNosEixo(arena_reset.get_jogador().get_x(), arena_reset.get_jogador().get_y(), arena_reset.get_jogador().direcao);
    }

    if (letras['u'] == 1)
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

int main(int argc, char **argv)
{
    //Carregando Dados

    arena_modelo.inputProcessing(argv[1]);
    arena_reset.inputProcessing(argv[1]);
    //arena_modelo.print();
    basesIni = arena_modelo.BasesInimigas();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("trabalho 4");
    init();

    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyup);
    glutPassiveMotionFunc(motion);
    glutMouseFunc(mouse);

    glutIdleFunc(idle);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}