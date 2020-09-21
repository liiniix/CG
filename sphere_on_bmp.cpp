#ifdef _WIN32
#include <windows.h>
#else

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#endif

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
	BITMAPFILEHEADER FILE;
	BITMAPINFOHEADER INFO;
	BYTE *DATA;
} BMPIMAGE;

BMPIMAGE IM; //bmp image data

typedef struct{
    BYTE R;
    BYTE G;
    BYTE B;
} pix; //3D point or vector

static pix **data;

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

static int Ym, Xm;

/* memory allocation */
void *mycalloc(int items, int size){
  void *ptr;
  ptr = calloc(items, size);
  if(ptr == NULL){
    fprintf(stderr,"can't allocate memory\n");
    exit(1);
  }
  return(ptr);
}


static int openImage(char *filename){
	FILE *fp;
	fp=fopen(filename,"r+b");
	if(fp==NULL){
        printf("no file\n");
        return 0;
	}
	fread(&IM.FILE,sizeof(BYTE),14,fp);
	fread(&IM.INFO,sizeof(BYTE),40,fp);
	IM.DATA = (BYTE *)malloc(IM.INFO.biWidth*IM.INFO.biHeight*3);
	fread(IM.DATA,sizeof(BYTE),IM.INFO.biWidth*IM.INFO.biHeight*3,fp);
	fclose(fp);
	return 1;
}

static void reShape(int width, int height){
    const float ar = (float)width/(float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Xm/2,Xm/2,-Ym/2,Ym/2,-1,1); //640x480
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
    sphere sp[3]={{100,230,0,-650},
                {100, -230,0,-650},
                {100, 0,0,-1010}}; // 3 spheres
    ver COP={0, 0, 0};
    ver Light={-200, 500, -300}; //light at the center of 3 spheres, but above 500
    ver P,N,L,Ce,V,H;
    float Zp = -500;
    float A,B,C,aH,ambi=0.2;
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
                    glColor3ub(data[y+Ym/2][x+Xm/2].R,data[y+Ym/2][x+Xm/2].G,data[y+Ym/2][x+Xm/2].B);
                   //glColor3f(0,0,0);
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
    int x,y;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glBegin(GL_POINTS);
    /*for(y=0;y<IM.INFO.biHeight;y++){
        for(x=0;x<IM.INFO.biWidth;x++){
            glColor3ub(data[y][x].R,data[y][x].G,data[y][x].B);
            glVertex2i(x-Xm/2,y-Ym/2);
        }
    }*/
    drawsphere();//drawing spheres
    glEnd();
    glPopMatrix();
    glFlush();
    //glutSwapBuffers();
}

static void idle(void){
    glutPostRedisplay();
}

static void myinit(void){
    glClearColor(0,0,0,1);
}

int main(int argc, char *argv[]){
    int x,y,i;
    if(openImage("Input.bmp")){// file opened
        Xm = IM.INFO.biWidth;
        Ym = IM.INFO.biHeight;
        printf("Xmax = %d, Ymax = %d \n", Xm, Ym);
   }
    else{
        printf("File not found!\n");
        exit(1);
    }


    data = (pix **)mycalloc(Ym, sizeof(pix *));
	for(y=0; y<Ym; y++){
       data[y] = (pix *)mycalloc(Xm, sizeof(pix));
	}
	i=0;
	for(y=0; y<Ym; y++){
        for(x=0; x<Xm; x++){
            data[y][x].B=IM.DATA[i];
            data[y][x].G=IM.DATA[i+1];
            data[y][x].R=IM.DATA[i+2];
            i+=3;
        }
	}



    glutInit(&argc, argv);
    glutInitWindowSize(Xm,Ym);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Ray Tracing");
    glutReshapeFunc(reShape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    myinit();
    glutMainLoop();

    return EXIT_SUCCESS;
}
