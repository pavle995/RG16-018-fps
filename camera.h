#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "types.h"

extern struct Camera camera;

extern long currentTime;

extern int moving;
extern int movingForward;
extern int movingBackward;
extern int movingLeftSide;
extern int movingRightSide;

/* Camera properties */
typedef struct Camera {
    Vec3f position;
    Vec3f rotation;
    Vec3f look;
    float fov;
    float clipNear;
    float clipFar;
} Camera;

/** Initializes camera properties */
void initCamera();

/** Calculate where the camera looks */
void calculateCamera();

#endif
