#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* TODO: remove GL dependancy from util */
#include <GL/glu.h>
#include <GL/gl.h>

#define M_PI 3.14159265358979323846

#ifndef UTIL_H
#define UTIL_H

/** Write the error message to terminal and close the program */
void  errorFatal(const char* message);

/** Allocate a new string and return it's adress */
char* newString(const char* string);

/** Delete the allocated string memory */
void  deleteString(const char* string);

/** Delete the old string and make a new one, the old pointer now points to the new string */
void replaceString(const char* oldStr, const char* newStr);

/** Convert degrees to radians */
float toRad(float degrees);

/** Generate a random float between given lower and upper bounds */
float randFloat(float lowerBound, float upperBound);

/** Calculates aspect ratio */
float calculateAspectRatio(int width, int height);

/** Test OpenGL errors, exit the program if there is any and write it to the terminal,
 *  Write info string to indicate on what part of the code the error occurred */
void testErrors(const char* infoString);

#endif
