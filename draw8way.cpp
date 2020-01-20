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
int status, flag;
int _x0, _y0, _x1, _y1;



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
    //glVertex2i(x,y);
    draw8way(x,y,zone);
    while(x<x1){
        if(d<0){
            x++;d+=dE;
        }
        else{
            x++;y++;
            d+=dNE;
        }
        //glVertex2i(x,y);
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



static void display(void){
    int x = 200, y = 100;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(-320, 0);
    glVertex2i(319, 0);
    glVertex2i(0, -240);
    glVertex2i(0, 239);
    glEnd();
    glBegin(GL_POINTS);
    if (status == 1) 
    {
        findZone(_x0, _y0, _x1, _y1);
    }
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

