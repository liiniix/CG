#if 0
Skip to content
Using University of Dhaka Mail with screen readers
Meet
New meeting
My meetingsNew
Hangouts

2 of 4
Program for drawing rainbow colored B-splines
Inbox
x

Ishtiaque Zahid <ruko97@gmail.com>
Attachments
12:54 PM (1 hour ago)
to Haider


Attachments area
#endif // 0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define TARGET 0

typedef struct {
    float x;
    float y;
} poi; //2D point or vector

typedef struct {
    float R;
    float G;
    float B;
} color; // Color

static int Ym = 960, Xm = 1280;

//static poi P[6] = { {-620, 450}, {-530, -400},
//                {-150, 400}, {0, -460},
//                {450, 420}, {630, -260} }; // 6 vertex

static poi P[7][6]; // For 7 colors, 6 points for each color

static color rainbow[7] = { {0.7, 0, 1.0 }, { 0.2, 0.0, 1.0 }, { 0.6, 0.6, 1.0 }, {0, 1, 0},
    { 1, 1, 0 }, { 1, 0.6, 0}, {1, 0, 0} };

static poi Ch[6] = { {-1.5, 1.7}, {-1.5, -1},{1.5, 1.7},
                    {1.5, -1}, {1.5, 1.7}, {1.5, -1.7} };

static void initValues() {
    P[0][0] = { -620, 450 }, P[0][1] = { -530, -400 }, P[0][2] = { -150, 400 },
        P[0][3] = { 0, -400 }, P[0][4] = { 450, 400 }, P[0][5] = { 630, -400 };

    for (int i = 1; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            P[i][j].x = P[i - 1][j].x + 6;
            P[i][j].y = P[i - 1][j].y + 6;
        }
    }
}

static void reShape(int width, int height) {
    //const float ar = (float)width/(float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Xm / 2, Xm / 2 - 1, Ym / 2 - 1, -Ym / 2, -1, 1); //640x480
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Draws border along the curve of target color
static void drawBorder(int target) {
    for (int i = 0; i < 6; i++) {
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(0, 1, 1);
        glVertex2i((int)P[target][i % 6].x, (int)P[target][i % 6].y);
        glVertex2i((int)P[target][(i + 1) % 6].x, (int)P[target][(i + 1) % 6].y);
        glVertex2i((int)P[target][(i + 2) % 6].x, (int)P[target][(i + 2) % 6].y);
        glVertex2i((int)P[target][(i + 3) % 6].x, (int)P[target][(i + 3) % 6].y);
        glEnd();

        glPointSize(1);
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex2i((int)P[target][i % 6].x, (int)P[target][i % 6].y);
        glVertex2i((int)P[target][(i + 1) % 6].x, (int)P[target][(i + 1) % 6].y);

        glVertex2i((int)P[target][(i + 1) % 6].x, (int)P[target][(i + 1) % 6].y);
        glVertex2i((int)P[target][(i + 2) % 6].x, (int)P[target][(i + 2) % 6].y);

        glVertex2i((int)P[target][(i + 2) % 6].x, (int)P[target][(i + 2) % 6].y);
        glVertex2i((int)P[target][(i + 3) % 6].x, (int)P[target][(i + 3) % 6].y);

        glEnd();
    }
}

static void drawSpline(int color) {
    float t;
    poi Po;
    for (int i = 0; i < 6; i++) {
        glPointSize(1);
        glColor3f(rainbow[color].R, rainbow[color].G, rainbow[color].B);
        glBegin(GL_POINTS);
        for (t = 0; t < 1.0; t += 0.0005) {
            Po.x = ((1 - t) * (1 - t) * (1 - t) * P[color][(i) % 6].x + (3 * t * t * t - 6 * t * t + 4) * P[color][(i + 1) % 6].x
                + (-3 * t * t * t + 3 * t * t + 3 * t + 1) * P[color][(i + 2) % 6].x + t * t * t * P[color][(i + 3) % 6].x) / 6;
            Po.y = ((1 - t) * (1 - t) * (1 - t) * P[color][(i) % 6].y + (3 * t * t * t - 6 * t * t + 4) * P[color][(i + 1) % 6].y
                + (-3 * t * t * t + 3 * t * t + 3 * t + 1) * P[color][(i + 2) % 6].y + t * t * t * P[color][(i + 3) % 6].y) / 6;
            glVertex2i((int)Po.x, (int)Po.y);
        }
        glEnd();
        glColor3f(1, 1, 1);
    }
}

static void drawSplines() {
    for (int c = 0; c < 7; c++) {
        drawSpline(c);
    }
}

// Points are bounced if the points having target color meet the corner of the screen
static void bouncePoints(int target) {
    for (int i = 0; i < 6; i++) {
        for (int c = 0; c < 7; c++) {
            P[c][i].x += Ch[i].x;
        }
        if (P[target][i].x > (Xm/2 - 1) || P[target][i].x < (-Xm/2)) Ch[i].x *= -1;
        for (int c = 0; c < 7; c++) {
            P[c][i].y += Ch[i].y;
        }
        if (P[target][i].y > (Ym/2 - 1) || P[target][i].y < (-Ym/2)) Ch[i].y *= -1;
    }
}

static void display(void) {
    for (;;) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        //drawBorder(TARGET);
        drawSplines();
        glPopMatrix();
        //glFlush();
        glutSwapBuffers();
        bouncePoints(TARGET);
    }
}

static void idle(void) {
    glutPostRedisplay();
}

static void myinit(void) {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_COLOR_MATERIAL);
}

static int nearPoi(poi a, poi b) {
    float d;
    d = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    if (d < 5.0) return 1;
    else return 0;
}

static void OnMouseClick(int button, int state, int X, int Y) {
    poi Pt;
    int i, k = -1;
    Pt.x = X - Xm / 2;
    Pt.y = Y - Ym / 2;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        //store the x,y value where the click happened
        puts("Left button clicked");
        for (i = 0; i < 4; i++) {
            if (nearPoi(P[TARGET][(i) % 4], Pt)) {
                k = i;
                break;
            }
        }
        printf("i = %d, x = %d and y = %d\n", k, X - Xm / 2, Y - Ym / 2);
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        //store the x,y value where the click happened
        puts("Right button clicked");
    }
    glutPostRedisplay();
}
int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(Xm, Ym);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Spline Curve");
    glutReshapeFunc(reShape);
    glutDisplayFunc(display);
    glutMouseFunc(OnMouseClick);
    glutIdleFunc(idle);
    myinit();
    initValues();
    glutMainLoop();

    return EXIT_SUCCESS;
}
