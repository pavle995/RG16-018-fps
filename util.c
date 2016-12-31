#include "util.h"

void errorFatal(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

char* newString(const char* string) {
    char* str = (char*) malloc((strlen(string)+1) * sizeof(char));
    if (str == NULL)
        errorFatal("String malloc...");
    strcpy(str, string);
    return str;
}

void deleteString(const char* string) {
    free((void*)string);
}

void replaceString(const char* oldStr, const char* newStr) {
    deleteString(oldStr);
    oldStr = newString(newStr);
}

float toRad(float degrees) {
    return degrees / (180.0 / M_PI);
}

float randFloat(float lowerBound, float upperBound) {
    float number = (float)rand() / RAND_MAX;

    return lowerBound + (number * (upperBound - lowerBound));
}

extern float calculateAspectRatio(int width, int height) {
    if (height == 0)
        height = 1;
    return (float)width / height;
}

void testErrors(const char* infoString) {
	GLuint errorNumber;
	errorNumber = glGetError();
	if (errorNumber != GL_NO_ERROR) {
		const GLubyte* errorMessage = gluErrorString(errorNumber);
		fprintf(stderr, "Error test for: [%s] -> (%u) %s\n", infoString, errorNumber, errorMessage);
		//free((void*)errorMessage);	// free string???
		exit(EXIT_FAILURE);
	}
}
