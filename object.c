#include "object.h"

Object* createObject(Vec3f* translation, Vec3f* rotation, Vec3f* scale, void (*drawFunction)(void)) {
    Object* object = NULL;
    object = (Object*) malloc(sizeof(Object));
    if (object == NULL)
        errorFatal("Object memory allocation failed...");

    Vec3f tmpTranslation = {translation->x, translation->y, translation->z};
    Vec3f tmpRotation    = {rotation->x,    rotation->y,    rotation->z};
    Vec3f tmpScale       = {scale->x,       scale->y,       scale->z};

    object->translation  = tmpTranslation;
    object->rotation     = tmpRotation;
    object->scale        = tmpScale;
    object->drawFunction = drawFunction;

    object->numberOfChildren = 0;

    return object;
}

void addObjectChild(Object* parent, Object* child) {
    Object** tmpPointerArray = NULL;

    if (parent->numberOfChildren == 0) {
        parent->children = (Object**) malloc(sizeof(Object*));
        if (parent->children == NULL)
            errorFatal("Object pointer array allocation failed...");
    }

    else {
        tmpPointerArray = (Object**) realloc(parent->children, sizeof(Object*) * (parent->numberOfChildren + 1));
        if (tmpPointerArray == NULL)
            errorFatal("Object pointer array reallocation failed...");
        parent->children = tmpPointerArray;
    }

    parent->children[parent->numberOfChildren] = child;
    parent->numberOfChildren++;
}

Object** createObjectArray(int numberOfObjects) {
    Object** objects = NULL;
    objects = (Object**) malloc(sizeof(Object*) * numberOfObjects);
    if (objects == NULL)
        errorFatal("Object array memory allocation failed...");

    return objects;
}
