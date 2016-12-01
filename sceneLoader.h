#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <stdio.h>
#include <string.h>

#include "util.h"
#include "object.h"
#include "drawShapes.h"

/** Loads objects from a file and adds them to the scene graph from root object */
void loadScene(Object** rootObject, const char* filepath);

#endif
