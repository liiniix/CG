/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <bits/stdc++.h>
using namespace std;

static int slices = 16;
static int stacks = 16;
int status, flag=0;
int _x0, _y0, _x1, _y1;
int TOP = 8, BOTTOM = 4, RIGHT = 2, LEFT = 1;
int xmax = 120, xmin = -120, ymax = 100, ymin = -100;




static void mouse(int button, int state, int mousex, int mousey)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
    {
        if (status != 0) 
        {
            _x0 = mousex - 320;
            _y0 = 240 - mousey;
            status = 0;
        } else if (status == 0) {
            _x1 = mousex - 320;
            _y1 = 240 - mousey;
            status = 1;
            flag = 0;
        }
    }
}

static void resize(int width, int height){
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-320, 319, -240, 239, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}



void draw8way(int x, int y, int zone){
    switch(zone){
        case 0:
        glVertex2i(x,y);
        break;
        case 1:
        glVertex2i(y, x);
        break;
        case 2:
        glVertex2i(-y,x);
        break;
        case 3:
        glVertex2i(-x,y);
        break;
        case 4:
        glVertex2i(-x,-y);
        break;
        case 5:
        glVertex2i(-y, -x);
        break;
        case 6:
        glVertex2i(y,-x);
        break;
        case 7:
        glVertex2i(x, -y);
        break;
        default:;
    }
}

void drawLine_0(int x0, int y0, int x1, int y1, int zone){
    glBegin(GL_POINTS);
    int x, y, d, dx, dy, dE, dNE;
    dx = x1-x0;
    dy = y1-y0;
    d = 2*dy-dx;
    dE = 2*dy;
    dNE = 2*(dy-dx);
    x = x0, y = y0;
    draw8way(x,y,zone);
    while(x<x1){
        if(d<0){
            x++;d+=dE;
        }
        else{
            x++;y++;
            d+=dNE;
        }
        draw8way(x,y,zone);
    }
    glEnd();
}


void findZone(int x0, int y0, int x1, int y1){
    int dx, dy;
    dx = x1-x0;
    dy = y1-y0;
    if(dx>=0 and dy>=0){
        if(dx>dy){
            //glColor3f(1.0, 0.0, 0.0);
            drawLine_0(x0,y0,x1,y1,0);
        }
        else{
            //glColor3f(0.0, 1.0, 0.0);
            drawLine_0(y0,x0,y1,x1,1);
        }
    }
    else if(dx<=0 and dy>=0){
        if(abs(dx) > dy){
            //glColor3f(0.0, 0.0, 1.0);
            drawLine_0(-x0,y0,-x1,y1,3);
        }
        else{
            //glColor3f(0.0, 0.0, 1.0);
            drawLine_0(y0,-x0,y1,-x1,2);
        }
    }
    else if(dx<=0 and dy<=0){
        if(abs(dx)>abs(dy)){
            //glColor3f(1.0, 1.0, 1.0);
            drawLine_0(-x0,-y0,-x1,-y1,4);
        }
        else{
            //glColor3f(1.0, 0.0, 1.0);
            drawLine_0(-y0,-x0,-y1,-x1,5);
        }
    }
    else if(dx>=0 and dy<=0){
        if(dx>abs(dy)){
            //glColor3f(0.0, 1.0, 1.0);
            drawLine_0(x0,-y0,x1,-y1,7);
        }
        else{
            //glColor3f(1.0, 1.0, 0.0);
            drawLine_0(-y0,x0,-y1,x1,6);
        }
    }
}
float S = 50;
struct P3D{
    float x, y, z;
    void init(float x_, float y_, float z_){
        x = x_;
        y = y_;
        z = z_;
    }
};
struct P2D{
    int x, y;
    void init(float x_, float y_){
        x = x_;
        y = y_;
    }
};
float Qdx = 0, Qdy = 0, Qdz = 100, zp = 0;
P3D P[8], Pt[8];
P2D p[8];
void project(){
    for(int i=0;i<8;++i){
        p[i].x = Pt[i].x + Pt[i].z*(-Qdx/Qdz) + zp*Qdx/Qdz;
        p[i].y = Pt[i].y + Pt[i].z*(-Qdy/Qdz) + zp*(Qdy/Qdz);
        float t = Pt[i].z*(-1/Qdz) + zp/Qdz + 1;
        p[i].x/=t;
        p[i].y/=t;
    }
}
float th = 0;
void rotate(){
    for(int i=0;i<8;++i){
        Pt[i].x = Pt[i].x*cos(th) - sin(th)*Pt[i].y;
        Pt[i].y = Pt[i].x*sin(th) + cos(th)*Pt[i].y;
    }
    printf("%f\n", Pt[0].x);
}
void abul(int i, int j){
    findZone(p[i].x, p[i].y, p[j].x, p[j].y);
}
void drawCube(){
    abul(0, 1);
    abul(1, 3);
    abul(3, 2);
    abul(2, 0);

    abul(4, 5);
    abul(5, 7);
    abul(7, 6);
    abul(6, 4);

    abul(1, 5);
    abul(0, 4);
    abul(3, 7);
    abul(2, 6);
}
void first(){
    Qdx = -3*S, Qdy = 4*S, Qdz = -4*S-100, zp = 0;
    float x = 0, y = 0, z = 0;
    P[0].init(x+S,y-S, z+S);
    P[1].init(x-S,y-S, z+S);
    P[2].init(x+S,y+S, z+S);
    P[3].init(x-S,y+S, z+S);

    P[4].init(x+S,y-S, z+-S);
    P[5].init(x-S,y-S, z+-S);
    P[6].init(x+S,y+S, z-S);
    P[7].init(x-S,y+S, z+-S);
}
static void display(void){
    int x = 200, y = 100;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    //glVertex2i(-320, 0);
    //glVertex2i(319, 0);
    //glVertex2i(0, -240);
    //glVertex2i(0, 239);
    //glVertex2i(xmax,ymax);
    //glVertex2i(xmax,ymin);
    //glVertex2i(xmin,ymax);
    //glVertex2i(xminth = (th+1)%360;,ymin);
    //glVertex2i(xmax,ymax);
    //glVertex2i(xmin,ymax);
    //glVertex2i(xmax,ymin);
    //glVertex2i(xmin,ymin);
    glVertex2i(-320, ymax);
    glVertex2i(319,ymax);
    glVertex2i(-320, ymin);
    glVertex2i(319,ymin);
    glVertex2i(xmin, 239);
    glVertex2i(xmin, -320);
    glVertex2i(xmax, 239);
    glVertex2i(xmax, -320);
    glEnd();
    
    for(int i=0;i<8;++i)Pt[i]=P[i];
    //rotate();
    project();
    drawCube();
    th += .1;
    if(th>360)
        th = .001;
    
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[]){
    status = -1;
    flag = 1;
    first();
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Experiment 01");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);

    glutMainLoop();

    return EXIT_SUCCESS;
}

