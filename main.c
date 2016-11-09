#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "util.h"

/* Float vector with 3 components */
typedef struct Vec3f {
    float x;
    float y;
    float z;
} Vec3f;

/* Camera properties */
typedef struct Camera {
    Vec3f position;
    Vec3f rotation;
    Vec3f look;
    float fov;
    float clipNear;
    float clipFar;
} Camera;

/* Simple object with transforms and a function pointer to a function that draws it */
typedef struct Object {
    Vec3f translation;
    Vec3f rotation;
    Vec3f scale;
    void  (*drawFunction)(void);
} Object;

/* Global states and variables */
int*   argumentCount;
char** arguments;

char*  windowName;
int    windowWidth;
int    windowHeight;
float  aspectRatio;

int key_w = 0;
int key_s = 0;
int key_a = 0;
int key_d = 0;
int key_up    = 0;
int key_down  = 0;
int key_left  = 0;
int key_right = 0;

Camera camera;

Object** objects;
int numberOfObjects = 0;


/** Initializes program arguments and window properties */
void initProgram(int* argc, char** argv);

/** Initializes GLUT states and registers callback functions */
void initGlut();

/** Starts GLUT main loop */
void startGlut();

/** Initializes OpenGL states and viewport */
void initGL();

/** Initializes world components */
void initWorld();

/** Initializes camera properties */
void initCamera();

/** Cleans up memory and exits program */
void exitProgram();

/** Create a vector structure with 3 floats */
Vec3f* createVec3f(float x, float y, float z);

/** Set the vectors components to new values */
void setVec3f(Vec3f* vec3f, float x, float y, float z);

/** Create an object with transforms and it's draw function */
Object* createObject(Vec3f* translation, Vec3f* rotation, Vec3f* scale, void (*drawFunction)(void));

/** Create an array of objects, this array is used in scene rendering,
  * a FOR loop goes through and draws all the objects */
Object** createObjectArray(int numberOfObjects);

/** Draw simple ground quad - for testing purposes */
void drawGround();

/** Draw a simple box with wireframe - for testing purposes */
void drawBox();

/** Draw a teapot - for testing purposes */
void drawTeapot();

/** Draw a simple low poly ball with wireframe - for testing purposes */
void drawBall();

/** Draw a simple wireframe sky sphere - for testing purposes */
void drawSkySphere();

/** Draws coordinate system arrow helper */
void drawCoordinateSystem();


/** Calculate camera angles with mouse movements */
void mouseLook(int mouseX, int mouseY);

/** Calculate where the camera looks */
void calculateCamera();

/** Calculate input events */
void calculateInput();


/** Calculates aspect ratio */
float calculateAspectRatio(int width, int height);

/** Handles basic ascii keyboard character press key states */
void keyboardBasicPress(unsigned char key, int mouseX, int mouseY);

/** Handles basic ascii keyboard character release key states */
void keyboardBasicRelease(unsigned char key, int mouseX, int mouseY);

/** Handles special keyboard character press key states */
void keyboardSpecialPress(int key, int mouseX, int mouseY);

/** Handles special keyboard character release key states */
void keyboardSpecialRelease(int key, int mouseX, int mouseY);

/** Handles mouse movement event */
void mouseMove(int mouseX, int mouseY);


/** Renders the 3D scene */
void displayScene();

/** Resizes the window and changes aspect ratio */
void resizeWindow(int width, int height);


int main(int argc, char** argv) {

    initProgram(&argc, argv);
    initGlut();

    initWorld();

    initGL();

    startGlut();

    return 0;
}


void initProgram(int* argc, char** argv) {
    argumentCount = argc;
    arguments     = argv;

    windowName = newString("Window name");
    windowWidth  = 800;
    windowHeight = 600;
    aspectRatio  = calculateAspectRatio(windowWidth, windowHeight);
}

void exitProgram() {
    deleteString(windowName);
}

void initCamera() {
    camera.position.x = 1.2;
    camera.position.y = 2.4;
    camera.position.z = 5.5;

    camera.rotation.x = 250.0;
    camera.rotation.y = 120.0;
    camera.rotation.z = 0.0;

    camera.fov      = 60.0;
    camera.clipNear = 0.01;
    camera.clipFar  = 100.0;
}

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

    return object;
}

Object** createObjectArray(int numberOfObjects) {
    Object** objects = NULL;
    objects = (Object**) malloc(sizeof(Object*) * numberOfObjects);
    if (objects == NULL)
        errorFatal("Object array memory allocation failed...");

    return objects;
}

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

void initWorld() {
    numberOfObjects = 6;
    objects = createObjectArray(numberOfObjects);

    Vec3f* tmpTranslation;
    Vec3f* tmpRotation;
    Vec3f* tmpScale;

    /* Sky sphere object */
    tmpTranslation = createVec3f(0, 0, 0);
    tmpRotation    = createVec3f(0, 0, 0);
    tmpScale       = createVec3f(1, 1, 1);
    objects[0] = createObject(tmpTranslation, tmpRotation, tmpScale, drawSkySphere);

    /* Ground object */
    setVec3f(tmpTranslation, 0, 0, 0);
    setVec3f(tmpRotation,    0, 0, 0);
    setVec3f(tmpScale,       1, 1, 1);
    objects[1] = createObject(tmpTranslation, tmpRotation, tmpScale, drawGround);

    /* Box object */
    setVec3f(tmpTranslation, -1.4, 0.5, -0.8);
    setVec3f(tmpRotation,     0,   35,   0);
    setVec3f(tmpScale,        1,   1,    1);
    objects[2] = createObject(tmpTranslation, tmpRotation, tmpScale, drawBox);

    /* Teapot object */
    setVec3f(tmpTranslation, -1.35, 1.1, -0.75);
    setVec3f(tmpRotation,     0,    0,    0);
    setVec3f(tmpScale,        0.2,  0.2,  0.2);
    objects[3] = createObject(tmpTranslation, tmpRotation, tmpScale, drawTeapot);

    /* Box object */
    setVec3f(tmpTranslation, -1.75, 1.05, -1.0);
    setVec3f(tmpRotation,     0,    70,    0);
    setVec3f(tmpScale,        0.1,  0.1,   0.1);
    objects[4] = createObject(tmpTranslation, tmpRotation, tmpScale, drawBox);

    /* Ball object */
    setVec3f(tmpTranslation,  0.8, 0.5, 1.1);
    setVec3f(tmpRotation,     0,   0,   0);
    setVec3f(tmpScale,        1,   1,   1);
    objects[5] = createObject(tmpTranslation, tmpRotation, tmpScale, drawBall);

    initCamera();
}

void drawObjects() {
    for (int i = 0; i < numberOfObjects; i++) {
        glPushMatrix();
            glTranslatef(objects[i][0].translation.x,
                         objects[i][0].translation.y,
                         objects[i][0].translation.z);
            glRotatef(objects[i][0].rotation.x, 1, 0, 0);
            glRotatef(objects[i][0].rotation.y, 0, 1, 0);
            glRotatef(objects[i][0].rotation.z, 0, 0, 1);
            glScalef(objects[i][0].scale.x,
                     objects[i][0].scale.y,
                     objects[i][0].scale.z);

            objects[i][0].drawFunction();
        glPopMatrix();
    }
}

void drawGround() {
    glColor3f(0.24, 0.4, 0.12);
    glBegin(GL_QUADS);
        glVertex3f(-2.0, 0.0, -2.0);
        glVertex3f(-2.0, 0.0,  2.0);
        glVertex3f( 2.0, 0.0,  2.0);
        glVertex3f( 2.0, 0.0, -2.0);
    glEnd();
}

void drawBox() {
    glColor3f(0.4, 0.29, 0.12);
    glutSolidCube(0.98);
    glColor3f(0.07, 0.06, 0.04);
    glLineWidth(2.0);
    glutWireCube(0.99);
}

void drawTeapot() {
    glColor3f(0.45, 0.46, 0.39);
    glutSolidTeapot(1.0);
}

void drawBall() {
    glColor3f(0.73, 0.22, 0.11);
    glutSolidSphere(0.495, 10, 8);
    glColor3f(0.26, 0.09, 0.05);
    glLineWidth(2.0);
    glutWireSphere(0.5, 10, 8);
}

void drawSkySphere() {
    glColor3f(0.08, 0.35, 0.71);
    glPushMatrix();
        glScalef(1.0, 0.5, 1.0);
        glRotatef(90, 1, 0, 0);
        glLineWidth(1.0);
        glutWireSphere(10, 20, 10);
    glPopMatrix();
}

void drawCoordinateSystem() {
    float size   = 1.0;
    float offset = 0.001;

    glLineWidth(3.0);
    glBegin(GL_LINES);
        /* X arrow line */
        glColor3f( 0.4,  0.0,    0.0);
        glVertex3f(0.0,  offset, 0.0);
        glVertex3f(size, offset, 0.0);

        /* Y arrow line */
        glColor3f( 0.0, 0.4,           0.0);
        glVertex3f(0.0, offset,        0.0);
        glVertex3f(0.0, size + offset, 0.0);

        /* Z arrow line */
        glColor3f( 0.0, 0.0, 0.4);
        glVertex3f(0.0, offset, 0.0);
        glVertex3f(0.0, offset, size);
    glEnd();
    glLineWidth(1.0);

    /* X arrow head */
    glColor3f(0.4, 0.0, 0.0);
    glPushMatrix();
        glTranslatef(size*0.9, 0, 0);

        glRotatef(45, 1, 0, 0);
        glScalef(2.0, 0.5, 0.5);
        glRotatef(45, 0, 1, 0);
        glutSolidCube(size*0.1);
    glPopMatrix();

    /* Y arrow head */
    glColor3f(0.0, 0.4, 0.0);
    glPushMatrix();
        glTranslatef(0, size*0.9, 0);

        glRotatef(45, 0, 1, 0);
        glScalef(0.5, 2.0, 0.5);
        glRotatef(45, 0, 0, 1);
        glutSolidCube(size*0.1);
    glPopMatrix();

    /* Z arrow head */
    glColor3f(0.0, 0.0, 0.4);
    glPushMatrix();
        glTranslatef(0, 0, size*0.9);

        glRotatef(45, 0, 0, 1);
        glScalef(0.5, 0.5, 2.0);
        glRotatef(45, 1, 0, 0);
        glutSolidCube(size*0.1);
    glPopMatrix();
}

float calculateAspectRatio(int width, int height) {
    if (height == 0)
        height = 1;
    return (float)width / height;
}

void initGlut() {
    /* Initialize GLUT */
    glutInit(argumentCount, arguments);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Create the window in the center of the screen */
    glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH)  /2 - windowWidth  /2,
                           glutGet(GLUT_SCREEN_HEIGHT) /2 - windowHeight /2);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(windowName);

    /* Register GLUT callback functions */
    glutKeyboardFunc(keyboardBasicPress);
    glutKeyboardUpFunc(keyboardBasicRelease);
    glutSpecialFunc(keyboardSpecialPress);
    glutSpecialUpFunc(keyboardSpecialRelease);
    glutPassiveMotionFunc(mouseMove);
    glutDisplayFunc(displayScene);
    glutIdleFunc(displayScene);
    glutReshapeFunc(resizeWindow);

    /* Set GLUT states */
    glutIgnoreKeyRepeat(1);
    glutSetCursor(GLUT_CURSOR_NONE);
}

void startGlut() {
    glutMainLoop();
}

void keyboardBasicPress(unsigned char key, int mouseX, int mouseY) {
    switch (key) {
        case 'w':
            key_w = 1;
            break;
        case 's':
            key_s = 1;
            break;
        case 'a':
            key_a = 1;
            break;
        case 'd':
            key_d = 1;
            break;

        /* Exit the program on ESC button press */
        case 27:
            glutLeaveMainLoop();
            exitProgram();
            break;
    }

    glutPostRedisplay();
}

void keyboardBasicRelease(unsigned char key, int mouseX, int mouseY) {
    switch (key) {
        case 'w':
            key_w = 0;
            break;
        case 's':
            key_s = 0;
            break;
        case 'a':
            key_a = 0;
            break;
        case 'd':
            key_d = 0;
            break;
    }

    glutPostRedisplay();
}

void keyboardSpecialPress(int key, int mouseX, int mouseY) {
    switch (key) {
        case GLUT_KEY_UP:
            key_up = 1;
            break;
        case GLUT_KEY_DOWN:
            key_down = 1;
            break;
        case GLUT_KEY_LEFT:
            key_left = 1;
            break;
        case GLUT_KEY_RIGHT:
            key_right = 1;
            break;
    }

    glutPostRedisplay();
}

void keyboardSpecialRelease(int key, int mouseX, int mouseY) {
    switch (key) {
        case GLUT_KEY_UP:
            key_up = 0;
            break;
        case GLUT_KEY_DOWN:
            key_down = 0;
            break;
        case GLUT_KEY_LEFT:
            key_left = 0;
            break;
        case GLUT_KEY_RIGHT:
            key_right = 0;
            break;
    }

    glutPostRedisplay();
}

void mouseMove(int mouseX, int mouseY) {
    /* Apply a simple hack to stop glutWarpPointer from freezing the program */
    static int warped = 0;

    if (warped) {
        warped = 0;
        return;
    }

    mouseLook(mouseX, mouseY);

    /* Place the cursor on the center of the window */
    glutWarpPointer(windowWidth / 2, windowHeight / 2);
    warped = 1;
}

void mouseLook(int mouseX, int mouseY) {
    float sensitivity = 0.2;

	/* How much the mouse moved */
	int dx = mouseX - windowWidth  / 2;
	int dy = mouseY - windowHeight / 2;

	/* Add mouse sensitivity and fix aspect ratio diference */
    camera.rotation.x = camera.rotation.x + (dx * sensitivity);
    camera.rotation.y = camera.rotation.y + (dy * sensitivity * aspectRatio);
}

void calculateCamera() {
    /* Limit camera angles */
    if (camera.rotation.x < 0)
        camera.rotation.x = camera.rotation.x + 360;
    else if (camera.rotation.x > 360)
        camera.rotation.x = camera.rotation.x - 360;

    if (camera.rotation.z < 0)
        camera.rotation.z = camera.rotation.z + 360;
    else if (camera.rotation.z > 360)
        camera.rotation.z = camera.rotation.z - 360;

    if (camera.rotation.y < 5)
        camera.rotation.y = 5;
    else if (camera.rotation.y > 175)
        camera.rotation.y = 175;

    /* Calculate camera normalized look vector from camera rotation */
    camera.look.x = sin(toRad(camera.rotation.y)) * cos(toRad(camera.rotation.x));
    camera.look.z = sin(toRad(camera.rotation.y)) * sin(toRad(camera.rotation.x));
    camera.look.y = cos(toRad(camera.rotation.y));

    gluLookAt(camera.position.x,                  camera.position.y,                  camera.position.z,
              camera.position.x + camera.look.x,  camera.position.y + camera.look.y,  camera.position.z + camera.look.z,
              0.0,                                1.0,                                0.0);
}

void calculateInput() {
    float step = 0.05;
    float turn = 2;

    /* Move camera forward */
    if (key_w == 1 && key_s == 0) {
        camera.position.x += camera.look.x * step;
        camera.position.y += camera.look.y * step;
        camera.position.z += camera.look.z * step;
    }

    /* Move camera backward */
    if (key_s == 1 && key_w == 0) {
        camera.position.x -= camera.look.x * step;
        camera.position.y -= camera.look.y * step;
        camera.position.z -= camera.look.z * step;
    }

    /* Move camera to the left side */
    if (key_a == 1 && key_d == 0) {
        camera.position.z -= camera.look.x * step;
        camera.position.x += camera.look.z * step;
    }

    /* Move camera to the right side */
    if (key_d == 1 && key_a == 0) {
        camera.position.z += camera.look.x * step;
        camera.position.x -= camera.look.z * step;
    }

    /* Turn the camera up */
    if (key_up == 1) {
        camera.rotation.y -= turn;
    }

    /* Turn the camera down */
    if (key_down == 1) {
        camera.rotation.y += turn;
    }

    /* Turn the camera left */
    if (key_left == 1) {
        camera.rotation.x -= turn;
    }

    /* Turn the camera right */
    if (key_right == 1) {
        camera.rotation.x += turn;
    }
}

void initGL() {
    glClearColor(0.29, 0.55, 0.57, 1.0);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.fov, aspectRatio, camera.clipNear, camera.clipFar);
}

void resizeWindow(int width, int height) {
    windowHeight = height;
    windowWidth  = width;

    aspectRatio = calculateAspectRatio(windowWidth, windowHeight);

    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.fov, aspectRatio, camera.clipNear, camera.clipFar);
}

void displayScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    calculateCamera();

    drawCoordinateSystem();

    drawObjects();

    calculateInput();

    glutSwapBuffers();
}
