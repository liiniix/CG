#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
    float x;
    float y;
} poi; //2D point or vector

static int Ym=960, Xm=1280;

static poi P[6]={{-620, 450}, {-530, -400},
                {-150, 400}, {0, -460},
                {450, 420}, {630, -260}}; // 4 vertex

static void reShape(int width, int height){
    //const float ar = (float)width/(float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Xm/2,Xm/2-1,Ym/2-1,-Ym/2,-1,1); //640x480
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void drawLine(int x0, int y0, int x1, int y1){

}

static void drawSpline(void){
    float t;
    poi Po;
    for(int i=0; i<6; i++){
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(0,1,1);
        glVertex2i((int)P[i%6].x, (int)P[i%6].y);
        glVertex2i((int)P[(i+1)%6].x, (int)P[(i+1)%6].y);
        glVertex2i((int)P[(i+2)%6].x, (int)P[(i+2)%6].y);
        glVertex2i((int)P[(i+3)%6].x, (int)P[(i+3)%6].y);
        glEnd();

        glPointSize(1);
        glBegin(GL_LINES);
        glColor3f(1,0,1);
        glVertex2i((int)P[i%6].x, (int)P[i%6].y);
        glVertex2i((int)P[(i+1)%6].x, (int)P[(i+1)%6].y);

        glVertex2i((int)P[(i+1)%6].x, (int)P[(i+1)%6].y);
        glVertex2i((int)P[(i+2)%6].x, (int)P[(i+2)%6].y);

        glVertex2i((int)P[(i+2)%6].x, (int)P[(i+2)%6].y);
        glVertex2i((int)P[(i+3)%6].x, (int)P[(i+3)%6].y);

        glEnd();

        glPointSize(1);
        glBegin(GL_POINTS);
        glColor3f(1,1,0);
        for(t=0; t<1.0; t+=0.001){
            Po.x = ((1-t)*(1-t)*(1-t)*P[(i)%6].x + (3*t*t*t - 6*t*t + 4)*P[(i+1)%6].x
                + (-3*t*t*t + 3*t*t + 3*t + 1)*P[(i+2)%6].x + t*t*t*P[(i+3)%6].x)/6;
            Po.y = ((1-t)*(1-t)*(1-t)*P[(i)%6].y + (3*t*t*t - 6*t*t + 4)*P[(i+1)%6].y
                + (-3*t*t*t + 3*t*t + 3*t + 1)*P[(i+2)%6].y + t*t*t*P[(i+3)%6].y)/6;
            glVertex2i((int)Po.x,(int)Po.y);
        }
        glEnd();
    }
}

static void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    /*for(y=0;y<IM.INFO.biHeight;y++){
        for(x=0;x<IM.INFO.biWidth;x++){
            glColor3ub(data[y][x].R,data[y][x].G,data[y][x].B);
            glVertex2i(x-Xm/2,y-Ym/2);
        }
    }*/
    drawSpline();
    glPopMatrix();
    glFlush();
    //glutSwapBuffers();
}

static void idle(void){
    glutPostRedisplay();
}

static void myinit(void){
    glClearColor(0,0,0,1);
    glEnable(GL_COLOR_MATERIAL);
}

static int nearPoi(poi a, poi b){
    float d;
    d = sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
    if(d<5.0) return 1;
    else return 0;
}

static void OnMouseClick(int button, int state, int X, int Y){
    poi Pt;
    int i, k = -1;
    Pt.x = X-Xm/2;
    Pt.y = Y-Ym/2;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        //store the x,y value where the click happened
        puts("Left button clicked");
        for(i=0; i<4; i++){
            if(nearPoi(P[(i)%4],Pt)){
                k = i;
                break;
            }
        }
        printf("i = %d, x = %d and y = %d\n", k, X-Xm/2,Y-Ym/2);
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        //store the x,y value where the click happened
        puts("Right button clicked");
    }
}
int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitWindowSize(Xm,Ym);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Ray Tracing");
    glutReshapeFunc(reShape);
    glutDisplayFunc(display);
    glutMouseFunc(OnMouseClick);
    glutIdleFunc(idle);
    myinit();
    glutMainLoop();

    return EXIT_SUCCESS;
}
