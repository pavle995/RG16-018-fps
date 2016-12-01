#include "sceneLoader.h"

/* #define DEBUG */

static int stackSize = 32;         /* Max parent-child object tree depth */
static int stackPos = 0;           /* Current object stack position */
static Object* parentStack[32];    /* Parent objects stack */

static void pushParent(Object* parent) {
    if (stackPos == stackSize)
        errorFatal("Parent object stack overflow");
    parentStack[stackPos] = parent;
    stackPos++;
}

static Object* popParent() {
    if (stackPos == 0)
        errorFatal("Parent object stack underflow");
    stackPos--;
    return parentStack[stackPos];
}


void loadScene(Object** rootObject, const char* filepath) {
    FILE* inputFile = fopen(filepath, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "file: %s\n", filepath);
        errorFatal("Error opening scene file");
    }

    int  lineLength = 200;
    char line[lineLength];

    char objectName[32];            /* Object name buffer */
    Vec3f translation;              /* Translation buffer */
    Vec3f rotation;                 /* Rotate buffer */
    Vec3f scale;                    /* Scale buffer */
    void (*drawFunction)(void);     /* Objects draw function */

    Object* currentParent;
    Object* potentialParent;
    int currentDepth = 0;
    int readDepth;

    #ifdef DEBUG
    printf("\n*** Loading objects: **********************************************************\n");
    #endif

    while (fgets(line, lineLength, inputFile) != NULL) {

        /* Skip comment lines */
        if (line[0] == '#')
            continue;

        /* Get object data */
        sscanf(line, "%*s %s %f, %f, %f %f, %f, %f %f, %f, %f", objectName,
                        &translation.x, &translation.y, &translation.z,
                        &rotation.x,    &rotation.y,    &rotation.z,
                        &scale.x,       &scale.y,       &scale.z);
        /* Check if object exists */
        if (strcmp("teapot", objectName) == 0) {
            drawFunction = drawTeapot;
        } else if (strcmp("ball", objectName) == 0) {
            drawFunction = drawBall;
        } else if (strcmp("box", objectName) == 0) {
            drawFunction = drawBox;
        } else {
            fprintf(stderr, "\nin line: %s", line);
            fprintf(stderr, "Object: %s\n", objectName);
            errorFatal("Unknown scene object");
        }

        /* Get scene root object */
        if (line[0] == '+') {
            *rootObject = createObject(&translation, &rotation, &scale, drawFunction);
            currentParent = *rootObject;
            currentDepth++;

            #ifdef DEBUG
            printf("Object: %s\n", objectName);
            #endif
        }

        /* Get scene objects */
        else if (line[0] == '-') {
            /* Count parent tree depth */
            readDepth = 1;
            while (line[readDepth] == '-') {
                readDepth++;
            }

            Object* newObject = createObject(&translation, &rotation, &scale, drawFunction);

            /* Get tree hight diference, positive when deeper in the parent tree */
            int difference = readDepth - currentDepth;

            if (difference > 1) {                       /* Can't go more than 1 level deeper per object */
                errorFatal("Parent tree depth can increase only by 1 per parent object");
            } else if (difference == 1) {               /* Go 1 level deeper */
                pushParent(currentParent);
                currentParent = potentialParent;
                currentDepth++;
            } else if (difference < 0) {                /* Go higher in the tree to the objects parent */
                int back = -difference;
                for (int i = 0; i < back; i++) {
                    currentParent = popParent();
                    currentDepth--;
                }
            } /* else - same as for the previous object (do nothing new) */

            addObjectChild(currentParent, newObject);

            #ifdef DEBUG
            for (int i = 0; i < currentDepth; i++) {
                printf("....");
            }
            printf("Object: %s\n", objectName);
            #endif

            potentialParent = newObject;
        }
    }

    #ifdef DEBUG
    printf("*** Done **********************************************************************\n");
    #endif

    fclose(inputFile);
}
