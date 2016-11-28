#ifndef DRAWSHAPES_H
#define DRAWSHAPES_H

#include <GL/glut.h>
#include <GL/gl.h>

/** Draw simple ground quad - for testing purposes */
void drawGround();

/** Draw a simple box with wireframe - for testing purposes */
void drawBox();

/** Draw a teapot - for testing purposes */
void drawTeapot();

/** Draw a simple low poly ball with wireframe - for testing purposes */
void drawBall();

/** Draws nothing - dummy function */
void drawEmpty();

/** Draws coordinate system arrow helper */
void drawCoordinateSystem();

/** Draw a simple wireframe sky sphere - for testing purposes */
void drawSkySphere();

#endif
