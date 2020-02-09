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
#include <unistd.h>

static int slices = 16;
static int stacks = 16;
double x=200, y=100;
double r = 30;
double flag = 0;
static void display();

struct Center{
    double  x = 30, y = 30;
    void init(double x_, double y_){
        x = x_;
        y = y_;
    }
};
struct Velocity{
    double x_dir = -1, y_dir = -1;
    void init(double x_dir_, double y_dir_){
        x_dir = x_dir_;
        y_dir = y_dir_;
        //printf("%f %f\n", x_dir, y_dir);
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
        //printf("%f %f\n", x_dir, y_dir);
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
    void norm(){
        double div = sqrt(x_dir*x_dir + y_dir*y_dir);
        x_dir = x_dir/div; y_dir = y_dir/div;
    }

};
struct Acceleration{
    double x_dir = 0, y_dir = 1;
    void init(double x_dir_, double y_dir_){
        x_dir = x_dir_;
        y_dir = y_dir_;
        //printf("%f %f\n", x_dir, y_dir);
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
        //printf("%f %f\n", x_dir, y_dir);
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
    void norm(){
        double div = sqrt(x_dir*x_dir + y_dir*y_dir);
        x_dir = x_dir/div; y_dir = y_dir/div;
    }

};
struct Ellipse{
    Center center;
    double a = 60;
    double b = 60;
    Velocity velocity;
    Acceleration accel;
    void init(double x,double y,double a_, double b_, double x_dir, double y_dir){
        center.init(x,y);
        velocity.init(x_dir, y_dir);
        a = a_;
        b = b_;
    }
    void draw8way(struct Center point){
            glVertex2i(point.x+center.x,point.y+center.y);


            glVertex2i(-point.x+center.x,point.y+center.y);

            glVertex2i(-point.x+center.x, -point.y+center.y);

            glVertex2i(point.x+center.x, -point.y+center.y);


    }

    void drawEllipse_1(){
        Center point;
        point.x = 0, point.y = b;
        double d = 4*b*b +a*a;
        draw8way(point);
        while(2*a*a*point.y -1>2*b*b*(point.x+1)){
            if(d<0){
                d += 4*b*b*(2*point.x+3);
                point.x++;
            }
            else{
                d+=4*(b*b*(2*point.x+3)+a*a*(2-2*point.y));
                point.x++, point.y--;
            }
            draw8way(point);
            printf("%f %f\n",point.x, point.y);
        }
        while(point.y>0){
            if(d<0){
                d += 4*(b*b*(2*point.x+2)+a*a*(3-2*point.y));
                point.x++;
                point.y--;
            }
            else{
                d += 4*a*a*(-2*point.y+3);
                point.y--;
            }
            draw8way(point);
        }
    }

    //void actionXboundary(){
    //    if(center.x + r >= 319 || center.x-r <= -320)
    //        velocity.negateX();
    //}
    //void actionYboundary(){
    //    if((int)(center.y+b)>239 || (int)(center.y-b)<-240){
    //        double ta = a;
    //        double tb = b;
    //        for(int i=0;i<50;++i){
    //            a = a + ta*i/100;
    //            b = b - tb*i/100;
    //            drawEllipse_1();
    //        }
    //        for(int i=0;i<50;++i){
    //            a = a - ta*i/100;
    //            b = b + tb*i/100;
    //            drawEllipse_1();
    //        }
    //        a = ta;
    //        b = tb;
    //        velocity.negateY();
    //    }
    //}
    //void actionCollide(Ellipse Ellipse2){
    //    if( sqrt((center.x-Ellipse2.center.x)*(center.x-Ellipse2.center.x) 
    //        +
    //        (center.y-Ellipse2.center.y)*(center.y-Ellipse2.center.y)) <= r+Ellipse2.r ){

    //            velocity.negate();

    //        }
    //}

    void updateCenter(){
        center.x += velocity.x_dir;
        center.y += velocity.y_dir;
        velocity.x_dir += accel.x_dir;
        velocity.y_dir += accel.y_dir;
    }
};
struct Ellipse Ellipse1, Ellipse2, Ellipse3;
struct Center center;
struct Velocity velocity;
void init(){
    Ellipse1.init(0, 0, 70, 70, 0, 0);
    printf("ellispe");
    Ellipse2 = Ellipse1;
    Ellipse3 = Ellipse1;

}

void timer(int a){
    glutPostRedisplay();
    glutTimerFunc(1000/20,timer,0);

}
    double _x0, _y0, _x1, _y1;




static void resize(int width, int height){
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-320, 319, -240, 239, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}



void drawAxis(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(-320, 0);
    glVertex2i(319, 0);
    glVertex2i(0, -240);
    glVertex2i(0, 239);
    glEnd();
}
int fly = 1;
int i = 0, up = 1;
int p=0, q=0;

int a = 80, b = 80;
void kicu(){
    Ellipse3.center = Ellipse1.center;
    if(p<4){
        
        Ellipse3.a += Ellipse3.a*.05;
        Ellipse3.b -= Ellipse3.b*.05;
            glBegin(GL_POINTS);
            Ellipse3.drawEllipse_1();
            glEnd();
            p++;
    }
    else if(q<4){
        Ellipse3.a += Ellipse3.a*.05;
        Ellipse3.b -= Ellipse3.b*.05;
            glBegin(GL_POINTS);
            Ellipse3.drawEllipse_1();
            glEnd();
            q++;
    }
    else{
        p = q = 0;
        fly = 1;
        Ellipse1.velocity.init(0,0);
        Ellipse1.accel.negate();
        if(up)up = 0;
        else up = 1;
        Ellipse3 = Ellipse1;
    }
}
static void display(void){
    drawAxis();
    double a = Ellipse1.a, b = Ellipse1.b;
    if(fly && (Ellipse1.center.y-b<=-240 && !up) || (Ellipse1.center.y+b>239 && up)){
        fly = 0;
    }
    if(fly){
        glBegin(GL_POINTS);
        Ellipse1.drawEllipse_1();
        glEnd();
        int speed;
        Ellipse telp;
        double a = 80, b = 80;
        Ellipse1.updateCenter();
    }
    else{
        kicu();
    }
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
    printf("%f %f", Ellipse1.center.x, Ellipse1.center.y);

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

