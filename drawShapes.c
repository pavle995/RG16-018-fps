#include "drawShapes.h"

extern unsigned int skyTexture;

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

    //glColor3f(0.08, 0.35, 0.71);
	glColor3f(1, 1, 1);
    glPushMatrix();
		glRotatef(90, 0, 1, 0);
        glScalef(10.0, 5.0, 10.0);
        //glLineWidth(1.0);
        //glutSolidSphere(10, 20, 10);
		drawRawModel(skySphere, FULL);
    glPopMatrix();
}


void drawGridCell(Cell cell) {
    float offset = 0.02;
    float coord  = 0.5 - offset;


    /* Select cell quad color */
    switch (cell) {
        case unusedCell:
            /* Don't draw an unusedCell */
            return;
        case emptyCell:
            /* Green cell quad */
            glColor3f(0.35, 0.9, 0.2);
            break;
        case blockedCell:
            /* Red cell quad */
            glColor3f(0.7, 0.15, 0.1);
            break;
        case unknownCell:
            /* Gray cell quad */
            glColor3f(0.6, 0.6, 0.6);
            break;
    }

    /* Draw cell quad */
    glBegin(GL_QUADS);
        glVertex3f(-coord, 0.04, -coord);
        glVertex3f(-coord, 0.04,  coord);
        glVertex3f( coord, 0.04,  coord);
        glVertex3f( coord, 0.04, -coord);
    glEnd();

    // /* Select cell outline color */
    // switch (cell) {
    //     case unusedCell:
    //         /* Don't draw an unusedCell */
    //         break;
    //     case emptyCell:
    //         /* Dark green cell outline */
    //         glColor3f(0.1, 0.3, 0.05);
    //         break;
    //     case blockedCell:
    //         /* Dark red cell outline */
    //         glColor3f(0.3, 0.0, 0.0);
    //         break;
    //     case unknownCell:
    //         /* Dark gray cell outline */
    //         glColor3f(0.15, 0.15, 0.15);
    //         break;
    // }
    //
    // /* Draw cell outline */
    // glLineWidth(3.0);
    // glBegin(GL_LINE_STRIP);
    //     glVertex3f(-coord, 0.05, -coord);
    //     glVertex3f(-coord, 0.05,  coord);
    //     glVertex3f( coord, 0.05,  coord);
    //     glVertex3f( coord, 0.05, -coord);
    // glEnd();
}

void drawLevelGrid(Grid* grid) {
    int startX = - grid->width  / 2;
    int startZ = - grid->height / 2;

    int light   = glIsEnabled(GL_LIGHTING);
    int blend   = glIsEnabled(GL_BLEND);
    int texture = glIsEnabled(GL_TEXTURE);
    if (light)
        glDisable(GL_LIGHTING);
    if (blend)
        glDisable(GL_BLEND);
    if (texture) {
        glDisable(GL_TEXTURE);
        glDisable(GL_TEXTURE_2D);
    }

    for (int i = 0; i < grid->height; i++) {
        for (int j = 0; j < grid->width; j++) {

            float offsetX;
            float offsetZ;
            if (camera.position.x > 0)
                offsetX = 0.5;
            else
                offsetX = -0.5;

            if (camera.position.z > 0)
                offsetZ = -0.5;
            else
                offsetZ = 0.5;

            int distanceX = abs(j + startX - (int)(camera.position.x + offsetX));
            int distanceZ = abs(i + startZ - (int)(camera.position.z - offsetZ));

            if (sqrt(distanceX*distanceX + distanceZ*distanceZ) > 8)
                continue;

            /* Draw grid squares */
            glPushMatrix();
            glTranslatef(startX + j, 0, startZ + i);
            /* Blocked square */
            if ((grid->data)[i][j] == blockedCell) {
                drawGridCell(blockedCell);
            }
            /* Empty square */
            else if ((grid->data)[i][j] == emptyCell) {
                drawGridCell(emptyCell);
            }
            /* Unused square */
            else if ((grid->data)[i][j] == unusedCell) {
                drawGridCell(unusedCell);
            }
            /* Unknown square (debug) */
            else {
                drawGridCell(unknownCell);
            }

            // float numberSpace = 0.05;
            // glTranslatef(0, 0.5, 0);
            // glTranslatef(-numberSpace, 0, -numberSpace);
            // glColor3f(0.0, 0.0, 0.0);
            // drawNumber(i);
            // glTranslatef(2*numberSpace, 0, 2*numberSpace);
            // glColor3f(0.4, 0.4, 0.4);
            // drawNumber(j);

            glPopMatrix();
        }
    }

    if (light)
        glEnable(GL_LIGHTING);
    if (blend)
        glEnable(GL_BLEND);
    if (texture) {
        glEnable(GL_TEXTURE);
        glEnable(GL_TEXTURE_2D);
    }
}

// TODO: Make a debug graph draw function

// void drawLevelGraph(Graph* graph, Grid* grid) {
//
// }
