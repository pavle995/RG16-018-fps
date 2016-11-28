#include "types.h"

Vec3f* createVec3f(float x, float y, float z) {
    Vec3f* vec3f = NULL;
    vec3f = (Vec3f*) malloc(sizeof(Vec3f));
    if (vec3f == NULL)
        errorFatal("Vec3f memory allocation failed...");

    vec3f->x = x;
    vec3f->y = y;
    vec3f->z = z;

    return vec3f;
}

void setVec3f(Vec3f* vec3f, float x, float y, float z) {
    vec3f->x = x;
    vec3f->y = y;
    vec3f->z = z;
}
