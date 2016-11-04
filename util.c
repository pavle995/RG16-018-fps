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

void replaceString(const char* old, const char* new) {
    deleteString(old);
    old = newString(new);
}

float toRad(float degrees) {
    return degrees / (180.0 / M_PI);
}
