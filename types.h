#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>

#include "util.h"

/* Float vector with 3 components */
typedef struct Vec3f {
    float x;
    float y;
    float z;
} Vec3f;

typedef struct Vertex {
	float x;
	float y;
	float z;
} Vertex;

typedef struct Triangle {
	int v0;
	int v1;
	int v2;
} Triangle;

typedef struct Model {
    Triangle* triangles;
    Vertex*   verticies;
    int       numberOfTriangles;
    int       numberOfVerticies;
} Model;

/** Create a vector structure with 3 floats */
Vec3f* createVec3f(float x, float y, float z);

/** Set the vectors components to new values */
void setVec3f(Vec3f* vec3f, float x, float y, float z);

#endif
