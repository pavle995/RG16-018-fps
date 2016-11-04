#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define M_PI 3.14159265358979323846

#ifndef UTIL_H
#define UTIL_H

/** Write the error message to terminal and close the program */
void  errorFatal(const char* message);

/** Allocate a new string and return it's adress */
char* newString(const char* string);

/** Delete the allocated string memory */
void  deleteString(const char* string);

/** Delete the old string and make a new one, the old pointer now points to the new string*/
void replaceString(const char* old, const char* new);

/** Convert degrees to radians */
float toRad(float degrees);

#endif  /* UTIL_H */
