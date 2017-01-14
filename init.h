#ifndef INIT_H
#define INIT_H

#include <stdlib.h>
#include <time.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "util.h"
#include "types.h"
#include "input.h"
#include "render.h"
#include "sceneLoader.h"
#include "modelLoader.h"

#include "imageTGA.h"

extern int* argumentCount;
extern char** arguments;

extern char* windowName;
extern int windowWidth;
extern int windowHeight;
extern float aspectRatio;

extern long currentTime;
extern int  deltaTime;

extern float randAxis[];

extern Model levelMain;
extern Model levelDecor;
extern Model skyModel;

extern Model testModel;
extern Model tree;
extern Model skySphere;

/** Initializes program arguments and window properties */
void initProgram(int* argc, char** argv);

/** Initializes GLUT states and registers callback functions */
void initGlut();

/** Starts GLUT main loop */
void startGlut();

/** Initializes OpenGL states and viewport */
void initGL();

/** Initializes world components */
void initWorld();

/** Cleans up memory and exits program */
void exitProgram();

#endif
