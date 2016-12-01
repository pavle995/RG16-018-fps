#include "init.h"

void initProgram(int* argc, char** argv) {
    srand(time(0));

    argumentCount = argc;
    arguments     = argv;

    windowName = newString("Window name");
    windowWidth  = 800;
    windowHeight = 600;
    aspectRatio  = calculateAspectRatio(windowWidth, windowHeight);
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

void initGL() {
    glClearColor(0.29, 0.55, 0.57, 1.0);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.fov, aspectRatio, camera.clipNear, camera.clipFar);
}

void initWorld() {
    numberOfObjects = 2;
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

    // /* Box object */
    // setVec3f(tmpTranslation, -1.4, 0.5, -0.8);
    // setVec3f(tmpRotation,     0,   35,   0);
    // setVec3f(tmpScale,        1,   1,    1);
    // objects[2] = createObject(tmpTranslation, tmpRotation, tmpScale, drawBox);
    //
    // /* Teapot object */
    // setVec3f(tmpTranslation, -1.35, 1.1, -0.75);
    // setVec3f(tmpRotation,     0,    0,    0);
    // setVec3f(tmpScale,        0.2,  0.2,  0.2);
    // objects[3] = createObject(tmpTranslation, tmpRotation, tmpScale, drawTeapot);
    //
    // /* Box object */
    // setVec3f(tmpTranslation, -1.75, 1.05, -1.0);
    // setVec3f(tmpRotation,     0,    70,    0);
    // setVec3f(tmpScale,        0.1,  0.1,   0.1);
    // objects[4] = createObject(tmpTranslation, tmpRotation, tmpScale, drawBox);
    //
    // /* Ball object */
    // setVec3f(tmpTranslation,  0.8, 0.5, 1.1);
    // setVec3f(tmpRotation,     0,   0,   0);
    // setVec3f(tmpScale,        1,   1,   1);
    // objects[5] = createObject(tmpTranslation, tmpRotation, tmpScale, drawBall);


    /* New SceneGraphObjects */
    /* Root object */
    loadScene(&root, "scene");


    // setVec3f(tmpTranslation,  2, 2, 2);
    // setVec3f(tmpRotation,     0,   0,   0);
    // setVec3f(tmpScale,        1,   1,   1);
    // root = createObject(tmpTranslation, tmpRotation, tmpScale, drawTeapot);
    //
    // setVec3f(tmpTranslation,  2, 2.2, 1);
    // setVec3f(tmpRotation,     0,   10,   0);
    // setVec3f(tmpScale,        0.8,   0.8,   0.8);
    // Object* objectBall1 = createObject(tmpTranslation, tmpRotation, tmpScale, drawBall);
    // addObjectChild(root, objectBall1);
    //
    // setVec3f(tmpTranslation,  2.2, 1.5, 1.6);
    // setVec3f(tmpRotation,     0,   30,   0);
    // setVec3f(tmpScale,        0.4,   0.4,   0.4);
    // Object* objectBall2 = createObject(tmpTranslation, tmpRotation, tmpScale, drawBall);
    // addObjectChild(root, objectBall2);
    //
    // for (int i = 0; i < numberOfFlyingObjects; i++) {
    //     setVec3f(tmpTranslation,  randFloat(0.25,2.5), randFloat(0.25,2.5), randFloat(0.25,2.5));
    //     setVec3f(tmpRotation,     randFloat(0,360),   randFloat(0,360),   randFloat(0,360));
    //     setVec3f(tmpScale,        randFloat(0.1,0.2),   randFloat(0.1,0.2),   randFloat(0.1,0.2));
    //     flyingObjects[i] = createObject(tmpTranslation, tmpRotation, tmpScale, drawBox);
    //     addObjectChild(objectBall1, flyingObjects[i]);
    // }
    //
    // for (int i = 0; i < numberOfFlyingObjects*3; i++) {
    //     randAxis[i] = randFloat(0,1);
    // }

    initCamera();
}

void exitProgram() {
    deleteString(windowName);
}
