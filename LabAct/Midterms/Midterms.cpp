#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__ //apple == mac
#include <GLUT/glut.h> //for mac
#else
#include <GL/glut.h> // for windows/linux
#endif
#include <iostream>
using namespace std;



void drawSky()
{
    glColor3f(0.5f, 0.8f, 1.0f);

    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -0.4f);
        glVertex2f( 1.0f, -0.4f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();
}

void drawGround()
{
    glColor3f(0.3f, 0.7f, 0.3f);

    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f, -0.4f);
        glVertex2f(-1.0f, -0.4f);
    glEnd();
}




void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
   drawGround();

    glFlush();
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("SceneShift");

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
