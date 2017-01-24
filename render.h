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

extern long currentTime;
extern int  deltaTime;

extern Object* root;
extern Camera camera;

extern void* font;

extern Object** objects;
extern int numberOfObjects;

extern Object* flyingObjects[];
extern int numberOfFlyingObjects;

extern unsigned int nullTexture;

extern int visualDebug;
extern int moving;
extern int movingForward;
extern int movingBackward;
extern int movingLeftSide;
extern int movingRightSide;

extern float mouseSideWave;
extern float mouseUpWave;

extern Model levelMain;
extern Model levelDecor;
extern Model skyModel;
extern Model gunModel;

extern Model testModel;
extern Model tree;
extern Model skySphere;

typedef enum ModelMode {
    POINTS, WIRE, FULL, COMBINED
} ModelMode;

/** Resizes the window and changes aspect ratio */
void resizeWindow(int width, int height);

/** Renders the 3D scene */
void displayScene();

/** Draws all objects from object array */
void drawObjects();

/** Recursively draws the object and all its children objects */
void drawSceneObjects(Object* object);

/** Draw a model with points or with wireframe or fully drawn */
void drawRawModel(Model model, ModelMode mode);

void drawDisplayListModel(Model model);

void setDrawing2D();

void setDrawing2DText();

void setDrawing3D();

void drawText(float x, float y, const char* string, int lineLength);

void drawTextLines(float x, float y, const char* string);

void drawTextLine(float x, float y, const char* string);


#endif
