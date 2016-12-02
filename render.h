#ifndef RENDER_H
#define RENDER_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "object.h"
#include "camera.h"
#include "util.h"
#include "drawShapes.h"
#include "input.h"          /* TODO change function structure, user input should not be used in render functions */

extern int windowHeight;
extern int windowWidth;
extern float aspectRatio;

extern Object* root;
extern Camera camera;

extern Object** objects;
extern int numberOfObjects;

extern Object* flyingObjects[];
extern int numberOfFlyingObjects;

extern Model testModel;

typedef enum ModelMode {
    POINTS, WIRE, FULL
} ModelMode;

/** Resizes the window and changes aspect ratio */
void resizeWindow(int width, int height);

/** Renders the 3D scene */
void displayScene();

/** Draws all objects from object array */
void drawObjects();

/** Recursively draws the object and all its children objects */
void drawSceneObjects(Object* object);

void drawModel(Model model, ModelMode mode);

#endif
