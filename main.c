#include <math.h>
#include <time.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "util.h"
#include "init.h"
#include "types.h"
#include "object.h"
#include "camera.h"
#include "drawShapes.h"

/* TODO: place all these global variables in a better place */

/* Global states and variables */
int*   argumentCount;
char** arguments;

char*  windowName;
int    windowWidth;
int    windowHeight;
float  aspectRatio;

long currentTime;
int  deltaTime;

Camera camera;

Object** objects;
int numberOfObjects = 0;

Object* flyingObjects[50];
int numberOfFlyingObjects = 50;
float randAxis[150];

Object* root;

/* Models used */
Model levelMain;
Model levelDecor;
Model skyModel;

Model testModel;
Model tree;
Model skySphere;    /* TODO: delete this */

int main(int argc, char** argv) {

    initProgram(&argc, argv);
    initGlut();

    initWorld();

    initGL();

	/* TODO: Organize main event loop, split it from glutMainLoop? */
    startGlut();

    return 0;
}
