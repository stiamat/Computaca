#ifndef _LINE_H
#define _LINE_H

class Line{
    //Inicio da Pista
    float x1, y1;

    //Fim da Pista
    float x2, y2;

    //Cor da Pista
    float r = 0, g = 0, b = 0;

    //Identificador da Pista
    int id = 0;
    
    public:
    
        Line();

        float get_x1();
        void set_x1(float p);
        float get_y1();
        void set_y1(float p);

        float get_x2();
        void set_x2(float p);
        float get_y2();
        void set_y2(float p);

        float get_r();
        void set_r(float p);

        float get_g();
        void set_g(float p);

        float get_b();
        void set_b(float p);

        int get_id();
        void set_id(int p);

};

#endif