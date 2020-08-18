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
#include<bits/stdc++.h>
#include <algorithm> 

static int slices = 16;
static int stacks = 16;
double _x0 = 0, _y0 = 0, _z0 = 500;
double color[650][490][3];
double a1=50,b1 =30, c1 =80, r1 = 70, a2 = 100, b2 = 0, c2 = 80, r2 = 50;

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
double max(double a, double b){
    if(a<b)
    return b;
    else return a;
}
static void display(void){
    int x = 10, y = 20;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(-320, 0);
    glVertex2i(319, 0);
    glVertex2i(0, -240);
    glVertex2i(0, 239);
    glEnd();
    glBegin(GL_POINTS);
    glVertex2i(x, y+1);
    glVertex2i(x, y);
    glEnd();


    for(double z1 = 50;z1<=90;z1+=1){
        for(int y1 = -240; y1<=239;y1+=1){
            for(int x1 = -320; x1<=319;x1+=1){
                double A1 = (x1-_x0)*(x1-_x0) + (y1-_y0)*(y1-_y0) + (z1-_z0)*(z1-_z0);
                double B1 = 2*( (_x0-a1)*(_x0-x1) + (_y0-b1)*(_y0-y1) + (_z0-c1)*(_z0-z1) );
                double C1 = (_x0-a1)*(_x0-a1) + (_y0-b1)*(_y0-b1) + (_z0-c1)*(_z0-c1) - r1*r1;

                double A2 = (x1-_x0)*(x1-_x0) + (y1-_y0)*(y1-_y0) + (z1-_z0)*(z1-_z0);
                double B2 = 2*( (_x0-a2)*(_x0-x1) + (_y0-b2)*(_y0-y1) + (_z0-c2)*(_z0-z1) );
                double C2 = (_x0-a2)*(_x0-a2) + (_y0-b2)*(_y0-b2) + (_z0-c2)*(_z0-c2) - r2*r2;
                double Lx = (_x0-x1), Ly = (_y0-y1), Lz = (_z0-z1);
                double Vx = (_x0-x1), Vy = (_y0-y1), Vz = (_z0-z1);
                double Hx = (Lx+Vx), Hy = (Ly+Vy), Hz = (Lz+Vz);
                double un = sqrt((Lx+Vx)*(Lx+Vx) + (Ly+Vy)*(Ly+Vy)+(Lz+Vz)*(Lz+Vz));
                Hx /=un, Hy/=un,Hz/=un;
                double t = 100000;
                int obj = 100;
                if(double D = B1*B1 - 4*A1*C1>=0){
                    if(-B1+sqrt(D)/(2*A1)<t){
                        t = -B1+sqrt(D)/(2*A1);
                        double Nx=x1-a1,Ny=y1-b1,Nz=z1-c1;
                        double un = sqrt(Nx*Nx+Ny*Ny+Nz*Nz);
                        Nx/=un, Ny/=un,Nz/=un;
                        color[x1+320][y1+240][0]=30+.5*(max(Nx*Lx+Ny*Ly+Nz*Lz,0.0)+pow(max(Nx*Hx+Ny*Hy+Nz*Hz,0.0),10));
                        //color[x1+320][y1+240][1]=.5*(max(Nx*Lx+Ny*Ly+Nz*Lz,0.0)+pow(max(Nx*Hx+Ny*Hy+Nz*Hz,0.0),100));
                        //color[x1+320][y1+240][2]=.5*(max(Nx*Lx+Ny*Ly+Nz*Lz,0.0)+pow(max(Nx*Hx+Ny*Hy+Nz*Hz,0.0),100));
                    }
                }
                if(double D = B2*B2 - 4*A2*C2>=0){
                    if(-B2+sqrt(D)/(2*A2)<t){
                        t = -B2+sqrt(D)/(2*A2);
                        color[x1+320][y1+240][0]=0;
                        double Nx=x1-a2,Ny=y1-b2,Nz=z1-c2;
                        double un = sqrt(Nx*Nx+Ny*Ny+Nz*Nz);
                        Nx/=un, Ny/=un,Nz/=un;
                        color[x1+320][y1+240][1]=30+.5*(max(Nx*Lx+Ny*Ly+Nz*Lz,0.0)+pow(max(Nx*Hx+Ny*Hy+Nz*Hz,0.0),10));
                        //color[x1+320][y1+240][0]=.5*(2*max(Nx*Lx+Ny*Ly+Nz*Lz,0.0)+3*pow(max(Nx*Hx+Ny*Hy+Nz*Hz,0.0),10));
                        //color[x1+320][y1+240][2]=.5*(2*max(Nx*Lx+Ny*Ly+Nz*Lz,0.0)+3*pow(max(Nx*Hx+Ny*Hy+Nz*Hz,0.0),10));
                    }
                }
            }
        }
    }

    for(int y1 = -240; y1<=239;y1+=1){
        for(int x1 = -320; x1<=319;x1+=1){
            glColor3ub(color[x1+320][y1+240][0],color[x1+320][y1+240][1],color[x1+320][y1+240][2]);
            
            glBegin(GL_POINTS);
            glVertex2i(x1,y1);
            glEnd();
            
        }
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
