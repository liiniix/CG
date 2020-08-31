#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#define Ym 480
#define Xm 640

typedef struct{
    float r;
    float a;
    float b;
    float c;
} sphere; //sphere parameter

typedef struct{
    float x;
    float y;
    float z;
} ver; //3D point or vector


static void reShape(int width, int height){
    const float ar = (float)width/(float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Xm/2,Xm/2-1,-Ym/2,Ym/2-1,-1,1); //640x480
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static ver vec(ver p1, ver p2){ //endpoint to vector
    ver V;
    float aV;
    V.x = p1.x - p2.x;
    V.y = p1.y - p2.y;
    V.z = p1.z - p2.z;
    aV=sqrt(V.x*V.x + V.y*V.y + V.z*V.z);
    V.x/=aV;
    V.y/=aV;
    V.z/=aV;
    return(V);
}

static float dot(ver a, ver b){ //dot product
    return(a.x*b.x + a.y*b.y + a.z*b.z);
}

static float mi(float a, float b){
    if(a<b) return a;
    else return b;
}

static float mx(float a, float b){
    if(a>b) return a;
    else return b;
}

static void drawsphere(void){
    sphere sp[3]={{200,230,0,-650},
                {200, -230,0,-650},
                {200, 0,0,-1010}}; // 3 spheres
    ver COP={0, 100, 0};
    ver Light={0, 500, -800}; //light at the center of 3 spheres, but above 500
    ver P,N,L,Ce,V,H;
    float Zp = -500;
    float A,B,C,aH,ambi=0.3;
    int x,y,s,i;
    float I, tmin, t, t1,t2;
    for(y= -Ym/2; y<Ym/2; y++){
        for(x= -Xm/2; x<Xm/2; x++){
            tmin = 100000.0;
            i = -1;
            for(s=0; s<3; s++){
                A = (x - COP.x)*(x - COP.x) + (y - COP.y)*(y - COP.y)
                + (Zp - COP.z)*(Zp - COP.z);
                B = 2*(COP.x-sp[s].a)*(x-COP.x) + 2*(COP.y-sp[s].b)*(y-COP.y)
                + 2*(COP.z-sp[s].c)*(Zp-COP.z);
                C = pow((COP.x-sp[s].a),2) + pow((COP.y-sp[s].b),2)
                +pow((COP.z-sp[s].c),2) -sp[s].r*sp[s].r;
                if(B*B < 4*A*C){
                    //no t will return
                }
                else if(B*B == 4*A*C){ //boundary
                    t = -B/(2*A);
                    if(t<tmin){
                       tmin=t;
                       i = s; //to remember sphere
                    }
                }
                else{//double intersection
                    t = (-B + sqrt(B*B - 4*A*C))/(2*A);
                    if(t<tmin){
                        tmin=t;
                        i = s;
                    }
                    t = (-B - sqrt(B*B - 4*A*C))/(2*A);
                    if(t<tmin){
                        tmin=t;
                        i = s;
                    }
                }
            }
            if(tmin == 100000.0){// no intersection
                   glColor3f(0,0,0);
                   glVertex2i(x,y);
            }
            else{
                if(i==0){ //1st sphere
                   P.x = COP.x + tmin*(x - COP.x);
                   P.y = COP.y + tmin*(y - COP.y);
                   P.z = COP.z + tmin*(Zp - COP.z);
                   L=vec(Light, P);
                   Ce.x=sp[i].a;
                   Ce.y=sp[i].b;
                   Ce.z=sp[i].c;
                   N=vec(P, Ce);
                   I=ambi+0.9*mx(dot(N,L),0);
                   glColor3f(I,0,I);//magenta
                   glVertex2i(x,y);
                }
                else if(i==1){ // 2nd sphere
                   P.x = COP.x + tmin*(x - COP.x);
                   P.y = COP.y + tmin*(y - COP.y);
                   P.z = COP.z + tmin*(Zp - COP.z);
                   L=vec(Light, P);
                   Ce.x=sp[i].a;
                   Ce.y=sp[i].b;
                   Ce.z=sp[i].c;
                   N=vec(P, Ce);
                   V = vec(COP, P);
                   H.x = L.x + V.x;
                   H.y = L.y + V.y;
                   H.z = L.z + V.z;
                   aH=sqrt(H.x*H.x + H.y*H.y + H.z*H.z);
                   H.x/=aH;
                   H.y/=aH;
                   H.z/=aH;
                   I=ambi + 0.9*mx(dot(N,L),0);//diffuse
                   A= pow(dot(N,H),50); //specular
                   glColor3f(mi(mx(I,A),1.0), mi(mx(I,A),1.0), A); //specular creates white-light, base=yellow
                   glVertex2i(x,y);
                }
                else{ //3rd sphere
                   P.x = COP.x + tmin*(x - COP.x);
                   P.y = COP.y + tmin*(y - COP.y);
                   P.z = COP.z + tmin*(Zp - COP.z);
                   L=vec(Light, P);
                   Ce.x=sp[i].a;
                   Ce.y=sp[i].b;
                   Ce.z=sp[i].c;
                   N=vec(P, Ce);
                   V = vec(COP, P);
                   H.x = L.x + V.x;
                   H.y = L.y + V.y;
                   H.z = L.z + V.z;
                   aH=sqrt(H.x*H.x + H.y*H.y + H.z*H.z);
                   H.x/=aH;
                   H.y/=aH;
                   H.z/=aH;
                   I=ambi + 0.9*mx(dot(N,L),0);//diffuse
                   A= pow(dot(N,H),50); //specular
                   glColor3f(A, mi(mx(I,A),1.0), mi(mx(I,A),1.0)); //specular creates white-light, base=yellow
                   glVertex2i(x,y);
                }
            }
        }
    }
}

static void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glBegin(GL_POINTS);
    drawsphere();//drawing spheres
    glEnd();
    glPopMatrix();
    glutSwapBuffers();
}

static void idle(void){
    glutPostRedisplay();
}

static void myinit(void){
    glClearColor(1,1,1,1);
}

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitWindowSize(Xm,Ym);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Ray Tracing");
    glutReshapeFunc(reShape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    myinit();
    glutMainLoop();
    return EXIT_SUCCESS;
}
