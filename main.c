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

/* Global states and variables */
int*   argumentCount;
char** arguments;

char*  windowName;
int    windowWidth;
int    windowHeight;
float  aspectRatio;

Camera camera;


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


/** Calculates aspect ratio */
float calculateAspectRatio(int width, int height);

/** Handles basic ascii keyboard character press events */
void keyboardBasicPress(unsigned char key, int mouseX, int mouseY);

/** Handles special keyboard character press events */
void keyboardSpecialPress(int key, int mouseX, int mouseY);

/** Renders the 3D scene */
void displayScene();

/** Resizes the window and changes aspect ratio */
void resizeWindow(int width, int height);


int main(int argc, char** argv) {

    initProgram(&argc, argv);
    initGlut();

    initWorld();
    initCamera();

    initGL();

    startGlut();

    return 0;
}


void initProgram(int* argc, char** argv) {
    argumentCount = argc;
    arguments     = argv;

    windowName = newString("Window name");
    windowWidth  = 640;
    windowHeight = 480;
    aspectRatio  = calculateAspectRatio(windowWidth, windowHeight);
}

void exitProgram() {
    deleteString(windowName);
}

void initCamera() {
    camera.position.x = 0.5;
    camera.position.y = 0.9;
    camera.position.z = 1.5;

    camera.rotation.x = 250.0;
    camera.rotation.y = 120.0;
    camera.rotation.z = 0.0;

    camera.fov      = 60.0;
    camera.clipNear = 0.1;
    camera.clipFar  = 100.0;
}

void initWorld() {

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
    glutSpecialFunc(keyboardSpecialPress);
    glutDisplayFunc(displayScene);
    glutIdleFunc(displayScene);
    glutReshapeFunc(resizeWindow);

    /* Set GLUT states */
    glutIgnoreKeyRepeat(0);
}

void startGlut() {
    glutMainLoop();
}

void keyboardBasicPress(unsigned char key, int mouseX, int mouseY) {
    float step = 0.05;
    switch (key) {
        case 'w':
            /* Move camera forward */
            camera.position.x += camera.look.x * step;
            camera.position.y += camera.look.y * step;
            camera.position.z += camera.look.z * step;
            break;
        case 's':
            /* Move camera backward */
            camera.position.x -= camera.look.x * step;
            camera.position.y -= camera.look.y * step;
            camera.position.z -= camera.look.z * step;
            break;
        case 'a':
            /* Move camera to the left side */
            camera.position.z -= camera.look.x * step;
            camera.position.x += camera.look.z * step;
            break;
        case 'd':
            /* Move camera to the right side */
            camera.position.z += camera.look.x * step;
            camera.position.x -= camera.look.z * step;
            break;
    }

    glutPostRedisplay();
}

void keyboardSpecialPress(int key, int mouseX, int mouseY) {
    float turn = 10;
    switch (key) {
        case GLUT_KEY_RIGHT:
            /* Turn the camera to the right */
            camera.rotation.x += turn;
            break;
        case GLUT_KEY_LEFT:
            /* Turn the camera to the left */
            camera.rotation.x -= turn;
            break;
        case GLUT_KEY_DOWN:
            /* Turn the camera up */
            camera.rotation.y += turn;
            break;
        case GLUT_KEY_UP:
            /* Turn the camera down */
            camera.rotation.y -= turn;
            break;

        glutPostRedisplay();
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
    else if (camera.rotation.y > 170)
        camera.rotation.y = 170;

    /* Calculate camera normalized look vector from camera rotation */
    camera.look.x = sin(toRad(camera.rotation.y)) * cos(toRad(camera.rotation.x));
    camera.look.z = sin(toRad(camera.rotation.y)) * sin(toRad(camera.rotation.x));
    camera.look.y = cos(toRad(camera.rotation.y));

    gluLookAt(camera.position.x,                  camera.position.y,                  camera.position.z,
              camera.position.x + camera.look.x,  camera.position.y + camera.look.y,  camera.position.z + camera.look.z,
              0.0,                                1.0,                                0.0);

    /* Sky-sphere wireframe */
    glColor3f(0.08, 0.35, 0.71);
    glPushMatrix();
        glScalef(1.0, 0.5, 1.0);
        glRotatef(90, 1, 0, 0);
        glutWireSphere(10, 20, 10);
    glPopMatrix();

    /* Draw coord system */
    glLineWidth(3.0);
    glBegin(GL_LINES);
      glColor3f(0.4, 0.0, 0.0);
      glVertex3f(0.0, 0.001, 0.0);
      glVertex3f(1.0, 0.001, 0.0);

      glColor3f(0.0, 0.4, 0.0);
      glVertex3f(0.0, 0.001, 0.0);
      glVertex3f(0.0, 1.001, 0.0);

      glColor3f(0.0, 0.0, 0.4);
      glVertex3f(0.0, 0.001, 0.0);
      glVertex3f(0.0, 0.001, 1.0);
    glEnd();
    glLineWidth(1.0);

    /* Draw ground */
    glBegin(GL_QUADS);
        glColor3f(0.24, 0.4, 0.12);
        glVertex3f(-0.5, 0.0, -0.5);
        glVertex3f(-0.5, 0.0,  0.5);
        glVertex3f( 0.5, 0.0,  0.5);
        glVertex3f( 0.5, 0.0, -0.5);
    glEnd();

    /* Draw a test box */
    glPushMatrix();
        glRotatef(35, 0.0, 0.1, 0.0);
        glTranslatef(0, 0.105, 0);
        glColor3f(0.4, 0.29, 0.12);
        glutSolidCube(0.2);
        glColor3f(0.11, 0.07, 0.03);
        glutWireCube(0.205);
    glPopMatrix();

    glutSwapBuffers();
}
