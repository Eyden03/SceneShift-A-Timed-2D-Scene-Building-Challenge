#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__ //apple == mac
#include <GLUT/glut.h> //for mac
#else
#include <GL/glut.h> // for windows/linux
#endif
#include <iostream>
using namespace std;

int timeOfDay = 0; 
int ground = 0;
int building = 0;


// SKY
void drawSky()
{
    if (timeOfDay == 0) // Day
        glColor3f(0.5f, 0.8f, 1.0f);

    else if (timeOfDay == 1) // Sunset
        glColor3f(1.0f, 0.5f, 0.3f);

    else if (timeOfDay == 2) // Twilight
        glColor3f(0.35f, 0.3f, 0.6f);

    else if (timeOfDay == 3) // Night
        glColor3f(0.05f, 0.08f, 0.2f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.4f);
    glVertex2f(1.0f, -0.4f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
}

// for the ground, we will use a simple green rectangle that covers the bottom part of the window
void drawGround()
{
    if (ground == 0) // Grass
        glColor3f(0.3f, 0.7f, 0.3f);

    else if (ground == 1) // Snow
        glColor3f(0.9f, 0.95f, 1.0f);

    else if (ground == 2) // Ice
        glColor3f(0.5f, 0.8f, 0.9f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.4f);
    glVertex2f(-1.0f, -0.4f);
    glEnd();
}

void drawHouse()
{
    // Main wall
    glColor3f(0.8f, 0.6f, 0.4f);
    glBegin(GL_QUADS);
        glVertex2f(-0.25f, -0.75f);
        glVertex2f( 0.25f, -0.75f);
        glVertex2f( 0.25f, -0.40f);
        glVertex2f(-0.25f, -0.40f);
    glEnd();

    // Roof
    glColor3f(0.5f, 0.2f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.32f, -0.40f);
        glVertex2f( 0.32f, -0.40f);
        glVertex2f( 0.00f, -0.15f);
    glEnd();

    // Door
    glColor3f(0.3f, 0.15f, 0.05f);
    glBegin(GL_QUADS);
        glVertex2f(-0.07f, -0.75f);
        glVertex2f( 0.07f, -0.75f);
        glVertex2f( 0.07f, -0.52f);
        glVertex2f(-0.07f, -0.52f);
    glEnd();
}

void drawCabin()
{
    // Cabin body - wider and darker than the house
    glColor3f(0.40f, 0.22f, 0.08f);
    glBegin(GL_QUADS);
        glVertex2f(-0.35f, -0.75f);
        glVertex2f( 0.35f, -0.75f);
        glVertex2f( 0.35f, -0.42f);
        glVertex2f(-0.35f, -0.42f);
    glEnd();

    // Roof
    glColor3f(0.20f, 0.10f, 0.05f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.43f, -0.42f);
        glVertex2f( 0.43f, -0.42f);
        glVertex2f( 0.00f, -0.12f);
    glEnd();

    // Chimney
    glColor3f(0.30f, 0.15f, 0.08f);
    glBegin(GL_QUADS);
        glVertex2f(0.20f, -0.27f);
        glVertex2f(0.27f, -0.27f);
        glVertex2f(0.27f, -0.10f);
        glVertex2f(0.20f, -0.10f);
    glEnd();

    // Door
    glColor3f(0.20f, 0.10f, 0.04f);
    glBegin(GL_QUADS);
        glVertex2f(-0.07f, -0.75f);
        glVertex2f( 0.07f, -0.75f);
        glVertex2f( 0.07f, -0.52f);
        glVertex2f(-0.07f, -0.52f);
    glEnd();

    // Left window
    glColor3f(0.85f, 0.75f, 0.35f);
    glBegin(GL_QUADS);
        glVertex2f(-0.27f, -0.63f);
        glVertex2f(-0.15f, -0.63f);
        glVertex2f(-0.15f, -0.51f);
        glVertex2f(-0.27f, -0.51f);
    glEnd();

    // Right window
    glBegin(GL_QUADS);
        glVertex2f(0.15f, -0.63f);
        glVertex2f(0.27f, -0.63f);
        glVertex2f(0.27f, -0.51f);
        glVertex2f(0.15f, -0.51f);
    glEnd();

    // Horizontal log lines
    glColor3f(0.25f, 0.12f, 0.04f);
    glBegin(GL_LINES);
        glVertex2f(-0.35f, -0.48f);
        glVertex2f( 0.35f, -0.48f);

        glVertex2f(-0.35f, -0.56f);
        glVertex2f( 0.35f, -0.56f);

        glVertex2f(-0.35f, -0.64f);
        glVertex2f( 0.35f, -0.64f);
    glEnd();
}

void drawObservatory()
{
    // Main building
    glColor3f(0.65f, 0.65f, 0.7f);
    glBegin(GL_QUADS);
        glVertex2f(-0.28f, -0.75f);
        glVertex2f( 0.28f, -0.75f);
        glVertex2f( 0.28f, -0.45f);
        glVertex2f(-0.28f, -0.45f);
    glEnd();

    // Simple dome
    glColor3f(0.8f, 0.8f, 0.85f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.28f, -0.45f);
        glVertex2f(-0.22f, -0.32f);
        glVertex2f(-0.12f, -0.23f);
        glVertex2f( 0.00f, -0.20f);
        glVertex2f( 0.12f, -0.23f);
        glVertex2f( 0.22f, -0.32f);
        glVertex2f( 0.28f, -0.45f);
    glEnd();
}


void drawBuilding()
{
    if (building == 1)
        drawHouse();

    else if (building == 2)
        drawCabin();

    else if (building == 3)
        drawObservatory();
}




void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawGround();
    drawBuilding();

    glFlush();
}






void keyboard(unsigned char key, int x, int y)
{
    // for time (sunset, twilight, night, day) change
    if (key == 's' || key == 'S')
    {
        timeOfDay++;

        if (timeOfDay > 3)
            timeOfDay = 0;

        glutPostRedisplay();
    }

    else if (key == 'a' || key == 'A')
    {
        building++;

        if (building > 3)
            building = 0;

        glutPostRedisplay();
    }

}

// for ground (grass, snow, ice) change
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (y >= 420)
        {
            ground++;

            if (ground > 2)
                ground = 0;

            glutPostRedisplay();
        }
    }
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("SceneShift");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutMainLoop();

    return 0;
}
