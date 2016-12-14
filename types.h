#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>

#include "util.h"

typedef struct Vec2f {
    float x;
    float y;
} Vec2f;

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
    Vec3f vn0;  /* TODO: make separate structures for this */
    Vec3f vn1;
    Vec3f vn2;
    Vec2f texcoord0;
    Vec2f texcoord1;
    Vec2f texcoord2;
    int smooth;
    int materialID;
} Triangle;

typedef struct Material {
    int id;

    float ambient[3];
    float diffuse[3];
    float specular[3];
    float emission[3];
    float shininess;

    float alpha;
    float illuminationModel;
} Material;

typedef struct Model {
    Triangle* triangles;
    Vertex*   verticies;
    Material* materials;
    int       numberOfTriangles;
    int       numberOfVerticies;
    int       numberOfMaterials;
} Model;

/** Create a vector structure with 3 floats */
Vec3f* createVec3f(float x, float y, float z);

/** Set the vectors components to new values */
void setVec3f(Vec3f* vec3f, float x, float y, float z);

#endif
