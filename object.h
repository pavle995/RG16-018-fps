#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glut.h>
#include <GL/gl.h>

#include "types.h"

extern Vec3f worldTranslate;
extern Vec3f worldRotate;
extern Vec3f worldScale;

typedef enum {
    EMPTY,
    OBJECT_3D
} ObjectType;

/* Simple object with transforms and a function pointer to a function that draws it */
typedef struct Object {
    ObjectType objectType;

    Vec3f origin;
    Vec3f translation;
    Vec3f rotation;
    Vec3f scale;

    void  (*drawFunction)(void);

    int    numberOfChildren;
    struct Object** children;
} Object;

/** Create an object with transforms and it's draw function */
Object* createObject(Vec3f* translation, Vec3f* rotation, Vec3f* scale, void (*drawFunction)(void));

/** Make the object child object of another parent object */
void addObjectChild(Object* parent, Object* child);

/** Create an array of objects, this array is used in scene rendering,
  * a FOR loop goes through and draws all the objects */
Object** createObjectArray(int numberOfObjects);

#endif
