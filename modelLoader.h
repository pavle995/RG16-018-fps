#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>

#include "project.h"
#include "types.h"
#include "util.h"
#include "imageTGA.h"

Model loadModel(const char* filename);

#endif
