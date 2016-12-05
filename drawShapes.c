#include "drawShapes.h"

void drawGround() {
    glColor3f(0.24, 0.4, 0.12);
    glBegin(GL_QUADS);
        glVertex3f(-4.0, 0.0, -4.0);
        glVertex3f(-4.0, 0.0,  4.0);
        glVertex3f( 4.0, 0.0,  4.0);
        glVertex3f( 4.0, 0.0, -4.0);
    glEnd();
}

void drawBox() {
    glColor3f(0.4, 0.29, 0.12);
    glutSolidCube(0.98);
    glColor3f(0.07, 0.06, 0.04);
    glLineWidth(2.0);
    glutWireCube(0.99);
}

void drawTeapot() {
    glLineWidth(1.0);
    glColor3f(0.45, 0.46, 0.39);
    glutSolidTeapot(1.0);
    glColor3f(0.25, 0.26, 0.19);
    glutWireTeapot(1.001);
}

void drawBall() {
    glColor3f(0.73, 0.22, 0.11);
    glutSolidSphere(0.495, 10, 8);
    glColor3f(0.26, 0.09, 0.05);
    glLineWidth(2.0);
    glutWireSphere(0.5, 10, 8);
}

void drawEmpty() {

}

void drawCoordinateSystem() {
    float size   = 1.0;
    float offset = 0.001;

    glLineWidth(3.0);
    glBegin(GL_LINES);
        /* X arrow line */
        glColor3f( 0.4,  0.0,    0.0);
        glVertex3f(0.0,  offset, 0.0);
        glVertex3f(size, offset, 0.0);

        /* Y arrow line */
        glColor3f( 0.0, 0.4,           0.0);
        glVertex3f(0.0, offset,        0.0);
        glVertex3f(0.0, size + offset, 0.0);

        /* Z arrow line */
        glColor3f( 0.0, 0.0, 0.4);
        glVertex3f(0.0, offset, 0.0);
        glVertex3f(0.0, offset, size);
    glEnd();
    glLineWidth(1.0);

    /* X arrow head */
    glColor3f(0.4, 0.0, 0.0);
    glPushMatrix();
        glTranslatef(size*0.7, 0, 0);
        glRotatef(90, 0, 1, 0);
        glutSolidCone(size*0.05, size*0.3, 3, 1);
    glPopMatrix();

    /* Y arrow head */
    glColor3f(0.0, 0.4, 0.0);
    glPushMatrix();
        glTranslatef(0, size*0.7, 0);
        glRotatef(-90, 1, 0, 0);
        glutSolidCone(size*0.05, size*0.3, 3, 1);
    glPopMatrix();


    /* Z arrow head */
    glColor3f(0.0, 0.0, 0.4);
    glPushMatrix();
        glTranslatef(0, 0, size*0.7);
        glRotatef(90, 0, 0, 1);
        glutSolidCone(size*0.05, size*0.3, 3, 1);
    glPopMatrix();

}

void drawSkySphere() {
    glColor3f(0.08, 0.35, 0.71);
    glPushMatrix();
        glScalef(1.0, 0.5, 1.0);
        glRotatef(90, 1, 0, 0);
        glLineWidth(1.0);
        glutWireSphere(10, 20, 10);
    glPopMatrix();
}
