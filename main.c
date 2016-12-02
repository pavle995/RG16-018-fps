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

/* Global states and variables */
int*   argumentCount;
char** arguments;

char*  windowName;
int    windowWidth;
int    windowHeight;
float  aspectRatio;

Camera camera;

Object** objects;
int numberOfObjects = 0;

Object* flyingObjects[50];
int numberOfFlyingObjects = 50;
float randAxis[150];

Object* root;

Model testModel;

int main(int argc, char** argv) {

    initProgram(&argc, argv);
    initGlut();

    initWorld();

    initGL();

    startGlut();

    return 0;
}
