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

static int slices = 16;
static int stacks = 16;

/* GLUT callback Handlers */

static void resize(int width, int height){
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-320, 319, -240, 239, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void drawLine_0(int x0, int y0, int x1, int y1){
    int x, y, d, dx, dy, dE, dNE;
    dx = x1-x0;
    dy = y1-y0;
    d = 2*dy-dx;
    dE = 2*dy;
    dNE = 2*(dy-dx);
    x = x0, y = y0;
    glVertex2i(x,y);
    while(x<x1){
        if(d<0){
            x++;d+=dE;
        }
        else{
            x++;y++;
            d+=dNE;
        }
        glVertex2i(x,y);
    }
}

void drawLine_1(int x0, int y0, int x1, int y1){
    int x, y, d, dx, dy, dN, dNE;
    dx = x1-x0;
    dy = y1-y0;
    d = dy - 2*dx;
    dN = -2*dy;
    dNE = 2*(dy-dx);
    y = y0, x = x0;
    glVertex2i(x,y);
    while(y<y1){
        if(d<0){
            x++;
            y++;
            d += dNE;
        }
        else{
            y++;
            d += dN;
        }
        glVertex2i(x,y);
    }
}
void drawLine_2(int x0, int y0, int x1, int y1){
    int x, y, d, dx, dy, dN, dNW;
    dx = x1 - x0, dy = y1 - y0;
    d = -dy-2*dx;
    dN = -2*dx;
    dNW = -2*dy + 2*dx;
    x = x0, y = y0;
    glVertex2i(x,y);
    while(y<=y1){
        if(d<0){
            y++;d += dN;
        }
        else{
            x--;
            y++;
            d += dNW;
        }
        glVertex2i(x, y);
    }
}
void drawLine_3(int x0, int y0, int x1, int y1){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3i(0,1,1);
    int x, y, d, dx, dy, dW, dNW;
    dx = x1 - x0, dy = y1 - y0;
    d = -2*dy-dx;
    dW = -2*dy;
    dNW = -2*dy - 2*dx;
    x = x0, y = y0;
    glVertex2i(x,y);
    while(x>=x1){
        if(d> 0){
            x--;d += dW;
        }
        else{
            x--;
            y++;
            d += dNW;
        }
        glVertex2i(x, y);
    }
}

void drawLine_4(int x0, int y0, int x1, int y1){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    int x, y, d, dx, dy, dW, dWS;
    dx = x1 - x0, dy = y1 - y0;
    d = -2*dy+dx;
    dW = -2*dy;
    dWS = -2*dy + 2*dx;
    x = x0, y = y0;
    glVertex2i(x,y);
    while(x>=x1){
        if(d< 0){
            x--;d += dW;
        }
        else{
            x--;
            y--;
            d += dWS;
        }
        glVertex2i(x, y);
    }
}
void drawLine_5(int x0, int y0, int x1, int y1){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1,0.5,0);
    int x, y, d, dx, dy, dS, dWS;
    dx = x1 - x0, dy = y1 - y0;
    d = -dy+2*dx;
    dS = 2*dx;
    dWS = -2*dy + 2*dx;
    x = x0, y = y0;
    glVertex2i(x,y);
    while(y>=y1){
        if(d>0){
            y--;d += dS;
        }
        else{
            x--;
            y--;
            d += dWS;
        }
        glVertex2i(x, y);
    }
}

void drawLine_6(int x0, int y0, int x1, int y1){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(.5,1,0);
    int x, y, d, dS, dSE, dx, dy;
    dx = x1-x0, dy = y1-y0;
    d = dy - 2*dx;
    dS = 2*dx;
    dSE = 2*(dy+dx);
    x = x0, y = y0;
    glVertex2i(x,y);
    while (y>=y1)
    {
        if(d<0){
            y--;
            d += dS;
        }
        else{
            x++,y--;
            d += dSE;
        }
        glVertex2i(x,y);
    }
}

void drawLine_7(int x0, int y0, int x1, int y1){
    int x, y, d, dx, dy, dE, dES;
    dx = x1 - x0;
    dy = y1 - y0;
    d = 2*dy+dx;
    dE = 2*dy;
    dES = 2*(dy+dx);
    x = x0, y = y0;
    glVertex2i(x,y);
    while(x<=x1){
        if(d<0){
            x++;y--;d += dES;
        }
        else{
            x++;
            d+=dE;
        }
        glVertex2i(x,y);

    }
}

void findZone(int x0, int y0, int x1, int y1){
    int dx, dy;
    dx = x1-x0;
    dy = y1-y0;
    if(dx>=0 and dy>=0){
        if(dx>dy)drawLine_0(x0,y0,x1,y1);
        else drawLine_1(x0,y0,x1,y1);
    }
    else if(dx<=0 and dy>=0){
        if(abs(dx) > dy)drawLine_3(x0,y0,x1,y1);
        else drawLine_2(x0,y0,x1,y1);
    }
    else if(dx<=0 and dy<=0){
        if(abs(dx)>abs(dy))drawLine_4(x0,y0,x1,y1);
        else drawLine_5(x0,y0,x1,y1);
    }
    else if(dx>=0 and dy<=0){
        if(dx>abs(dy))drawLine_7(x0,y0,x1,y1);
        else drawLine_6(x0,y0,x1,y1);
    }
}



static void display(void){
    int x = 10, y = 20;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(.5,1,.5);
    glBegin(GL_LINES);
    glVertex2i(-320, 0);
    glVertex2i(319, 0);
    glVertex2i(0, -240);
    glVertex2i(0, 239);
    glEnd();
    glBegin(GL_POINTS);
        findZone(0,0,100,10);
        findZone(0,0,10,100);
        findZone(0,0,-10, 100);
        findZone(0,0,-100, 10);
        findZone(0,0,100,-90);
        findZone(0,0,-100,-10);
        findZone(0,0,50,-160);
        findZone(0,0,-10,-160);
    glEnd();
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
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Experiment 01");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
