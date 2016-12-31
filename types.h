#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>

#include "util.h"

/* TODO: make better vector access, like an array */
/* Float vector with 2 components */
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

/* TODO: Move these to a separate file, triangles, materials and models are not basic types */
typedef struct Triangle {
	int v0;
	int v1;
	int v2;
    Vec3f vn0;  	/* TODO: make separate structures for this */
    Vec3f vn1;
    Vec3f vn2;
    Vec2f texcoord0;
    Vec2f texcoord1;
    Vec2f texcoord2;
    int smooth;
    unsigned int materialID;
} Triangle;

typedef struct Material {
    int id;

    float ambient[3];
    float diffuse[3];
    float specular[3];
    float emission[3];
    float shininess;

    float alpha;	/* TODO check how to use this in rendering */
    float illuminationModel;

	unsigned int textureID;
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

/** Delete vector3 */
void deleteVec3f(Vec3f* vec3f);

/* TODO: free vector2 memory function */

/** Set the vectors components to new values */
void setVec3f(Vec3f* vec3f, float x, float y, float z);

/* TODO: free model memory function*/

#endif
