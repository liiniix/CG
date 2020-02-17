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
#include <unistd.h>
#include <math.h>

static int slices = 16;
static int stacks = 16;
int x=200, y=100;
int r = 30;
int flag = 0;

struct Center{
    int  x = 30, y = 40;
    void init(int x_, int y_){
        x = x_;
        y = y_;
    }
};
struct Velocity{
    int x_dir = -1, y_dir = -1;
    void init(int x_dir_, int y_dir_){
        x_dir = x_dir_;
        y_dir = y_dir_;
    }
    void negate(){
        x_dir = -x_dir;
        y_dir = -y_dir;
    }
    void negateX(){
        x_dir = -x_dir;
    }
    void negateY(){
        y_dir = -y_dir;
    }
    void print(){
        printf("%d %d\n", x_dir, y_dir);
    }
    void add(struct Velocity velocity2){
        x_dir += velocity2.x_dir;
        y_dir += velocity2.y_dir;
    }
    double value(){
        double div = sqrt(x_dir*x_dir + y_dir*y_dir);
        struct Velocity rtn;
        return div;
    }
    struct Velocity norm(){
        double div = sqrt(x_dir*x_dir + y_dir*y_dir);
        struct Velocity rtn;
        rtn.init(x_dir/div, y_dir/div);
        return rtn;
    }

};
struct Circle{
    Center center;
    int r = 40;
    Velocity velocity;
    void init(int x,int y,int r_,int x_dir, int y_dir){
        center.init(x,y);
        r = r_;
        velocity.init(x_dir, y_dir);
    }
    void draw8way(struct Center point){
            glVertex2i(point.x + center.x,  point.y + center.y);
            glVertex2i(point.y + center.x, point.x + center.y);
            glVertex2i(-point.y + center.x, point.x + center.y);
            glVertex2i(-point.x + center.x, point.y + center.y);
            glVertex2i(-point.x + center.x, -point.y + center.y);
            glVertex2i(-point.y + center.x, -point.x + center.y);
            glVertex2i(point.y + center.x, -point.x + center.y);
            glVertex2i(point.x + center.x, -point.y + center.y);

    }

    void drawCircle_1(){
        Center point;
        point.x = 0;
        point.y = r;
        int d = 5 - 4*r;
        draw8way(point);
        while(point.x < point.y){
            if( d<0 ){
                d += 8*point.x+12;
                point.x++;
            }
            else{
                d += 8*point.x-8*point.y+20;
                point.x++;
                point.y--;
            }
            draw8way(point);
        }
    }

    void drawCircle_2(){
        Center point;
        point.x = r, point.y = 0;
        int d = 5 - 4*r;
        draw8way(point);
        while(point.y<point.x){
            if(d<0){
                d+=8*point.y+12;
                point.y++;
            }
            else{
                d += 8*point.y-8*point.x+20;
                point.x--;
                point.y++;
            }
            draw8way(point);
        }
    }
    void actionXboundary(){
        if(center.x + r >= 319 || center.x-r <= -320)
            velocity.negateX();
    }
    void actionYboundary(){
        if(center.y+r>=239 || center.y-r<=-240)
        velocity.negateY();
    }
    void actionCollide(Circle circle2){
        if( sqrt((center.x-circle2.center.x)*(center.x-circle2.center.x) 
            +
            (center.y-circle2.center.y)*(center.y-circle2.center.y)) <= 2*r ){

                velocity.negate();

            }
    }

    void updateCenter(){
        center.x += velocity.x_dir;
        center.y += velocity.y_dir;
    }
};
struct Circle circle1, circle2;
struct Center center;
struct Velocity velocity;
void init(){
    center.x = 30;
    center.y = 40;
    circle1.init(30, 40, 40, 2, -2);
    //circle2.init(-100,50,40,1,2);
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



static void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(-320, 0);
    glVertex2i(319, 0);
    glVertex2i(0, -240);
    glVertex2i(0, 239);
    glEnd();

    

    
    
        glBegin(GL_POINTS);
        circle1.drawCircle_2();
        glEnd();
        glutSwapBuffers();
        circle1.actionXboundary();
        circle1.actionYboundary();
        circle1.updateCenter();
        glutSwapBuffers();
}


static void key(unsigned char key, int x, int y){
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

    init();

    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Experiment 01");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    //glutTimerFunc(1000,timer, 0);
    //glutMouseFunc(mouse);

    glutMainLoop();



    return EXIT_SUCCESS;
}

