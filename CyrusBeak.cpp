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

#include <stdlib.h>
#include <stdio.h>

static int slices = 16;
static int stacks = 16;
int status, flag;
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
            flag = 1;
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
}


void findZone(int x0, int y0, int x1, int y1){
    int dx, dy;
    dx = x1-x0;
    dy = y1-y0;
    if(dx>=0 and dy>=0){
        if(dx>dy){
            glColor3f(1.0, 0.0, 0.0);
            drawLine_0(x0,y0,x1,y1,0);
        }
        else{
            glColor3f(0.0, 1.0, 0.0);
            drawLine_0(y0,x0,y1,x1,1);
        }
    }
    else if(dx<=0 and dy>=0){
        if(abs(dx) > dy){
            glColor3f(0.0, 0.0, 1.0);
            drawLine_0(-x0,y0,-x1,y1,3);
        }
        else{
            glColor3f(0.0, 0.0, 1.0);
            drawLine_0(y0,-x0,y1,-x1,2);
        }
    }
    else if(dx<=0 and dy<=0){
        if(abs(dx)>abs(dy)){
            glColor3f(1.0, 1.0, 1.0);
            drawLine_0(-x0,-y0,-x1,-y1,4);
        }
        else{
            glColor3f(1.0, 0.0, 1.0);
            drawLine_0(-y0,-x0,-y1,-x1,5);
        }
    }
    else if(dx>=0 and dy<=0){
        if(dx>abs(dy)){
            glColor3f(0.0, 1.0, 1.0);
            drawLine_0(x0,-y0,x1,-y1,7);
        }
        else{
            glColor3f(1.0, 1.0, 0.0);
            drawLine_0(-y0,x0,-y1,x1,6);
        }
    }
}
int makeCode(int x, int y){
    int Code = 0;
    if(y>ymax)
        Code += 8;
    if(y<ymin)
        Code += 4;
    if(x>xmax)
        Code += 2;
    if (x<xmin)
        Code += 1;
    return Code;
    
}

void CohenSutherland(int x0, int y0, int x1, int y1){
    int Code0, Code1, Code;
    int x, y;
    Code0 = makeCode(x0, y0);
    Code1 = makeCode(x1, y1);
    while(1){
        if(!(Code0|Code1)){
            findZone(x0, y0, x1, y1);
            break;
        }
        else if(Code0 & Code1){
            break;
        }
        else{
            if(Code0)Code=Code0;
            else Code = Code1;
            if(Code&TOP){
                y = ymax;
                x = x0 + (ymax-y0)*(x1-x0)/(y1-y0);
            }
            else if(Code & BOTTOM){
                y = ymin;
                x = x0 + (ymin-y0)*(x1-x0)/(y1-y0);
            }
            else if(Code & RIGHT){
                x = xmax;
                y = y0+(xmax -x0)*(y1-y0)/(x1-x0);
            }
            else if(Code & LEFT){
                x = xmin;
                y = y0 + (xmin-x0)*(y1-y0)/(x1-x0);
            }
            if(Code == Code0){
                x0 = x; y0 = y;
                Code0 = makeCode(x0, y0);
            }
            else{
                x1 = x; y1=y;
                Code1 = makeCode(x1,y1);
            }
        }
    }
}
void CyrusBeck(int x0, int y0, int x1, int y1){
    double te, temax, tlmin, tl, t;
    temax = 0, tlmin = 1;
    t  = (ymax-y0)/(y1-y0);//TOP
    printf("%f %f\n", t, tlmin);
    if(y1>y0){
        if(t<tlmin)
            tlmin = t;
        else{
            if(t>temax)
            temax = t;
        }
    }
    t = (ymin-y0)/(y1-y0);
    if(y0>y1){
        if(t<tlmin)
            tlmin = t;
        else{
            if(t>temax)
                temax = t;
        }
    }
    t = (xmax - x0)/(x1-x0);
    if(x1>x0){
        if(t<tlmin)
            tlmin = t;
        else{
            if(t>temax)
                temax = t;
        }
    }
    t = (xmin-x0)/(x1-x0);
    if(x0>x1){
        if(t<tlmin)
            tlmin = t;
        else{
            if(t>temax)
                temax = t;
        }
    }
    if(tlmin>=temax){
        printf("ok\n");
        int nx0 = x0 + temax*(x1-x0);
        int ny0 = y0 + temax*(y1-y0);
        int nx1 = x0 + tlmin*(x1-x0);
        int ny1 = y0 + tlmin*(y1-y0);
        findZone(nx0, ny0, nx1, ny1);
    }
}


static void display(void){
    int x = 200, y = 100;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(-320, 0);
    glVertex2i(319, 0);
    glVertex2i(0, -240);
    glVertex2i(0, 239);
    glVertex2i(xmax,ymax);
    glVertex2i(xmax,ymin);
    glVertex2i(xmin,ymax);
    glVertex2i(xmin,ymin);
    glVertex2i(xmax,ymax);
    glVertex2i(xmin,ymax);
    glVertex2i(xmax,ymin);
    glVertex2i(xmin,ymin);
    glEnd();
    if(status==1){
        glPointSize(3);
        glBegin(GL_POINTS);
        CyrusBeck(_x0, _y0, _x1, _y1);
        glEnd();
        glPointSize(1);
        glBegin(GL_POINTS);
        //findZone(_x0, _y0, _x1, _y1);
        glEnd();
    }
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

