#ifndef INPUT_H
#define INPUT_H

#include <GL/glut.h>

#include "project.h"
#include "init.h"
#include "camera.h"  

extern int windowHeight;
extern int windowWidth;
extern float aspectRatio;
extern Camera camera;

extern void glutLeaveMainLoop(void);

/** Calculate camera angles with mouse movements */
void mouseLook(int mouseX, int mouseY);

/** Handles mouse movement event */
void mouseMove(int mouseX, int mouseY);

/** Calculate input events */
void calculateInput();

/** Handles basic ascii keyboard character press key states */
void keyboardBasicPress(unsigned char key, int mouseX, int mouseY);

/** Handles basic ascii keyboard character release key states */
void keyboardBasicRelease(unsigned char key, int mouseX, int mouseY);

/** Handles special keyboard character press key states */
void keyboardSpecialPress(int key, int mouseX, int mouseY);

/** Handles special keyboard character release key states */
void keyboardSpecialRelease(int key, int mouseX, int mouseY);

#endif
