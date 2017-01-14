#include "init.h"

unsigned int nullTexture;
unsigned int skyTexture;

void initProgram(int* argc, char** argv) {
    srand(time(0));

    argumentCount = argc;
    arguments     = argv;

    windowName = newString("Window name");
    windowWidth  = 800;
    windowHeight = 600;
    aspectRatio  = calculateAspectRatio(windowWidth, windowHeight);
}

void glutSetOption ( GLenum eWhat, int value );

void initGlut() {
    /* Initialize GLUT */
    glutInit(argumentCount, arguments);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_SINGLE | GLUT_MULTISAMPLE); // | GLUT_MULTISAMPLE

    /* Create the window in the center of the screen */
    glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH)  /2 - windowWidth  /2,
                           glutGet(GLUT_SCREEN_HEIGHT) /2 - windowHeight /2);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(windowName);

    /* Get the current passed time */
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime   = 0;

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

	glutSetOption(GLUT_MULTISAMPLE, 4);
}

void startGlut() {
    glutMainLoop();
}

void initSky() {
	InternalImage* image;
	image = imageLoadTGA("images/sky_dome.tga");

	glGenTextures(1, &skyTexture);

	glBindTexture(GL_TEXTURE_2D, skyTexture);
	printf("Binded texture to ID: %d\n", skyTexture);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_LIENAR
	if (image->dataFormat == FormatRGB) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
					 image->width, image->height, 0,
					 GL_RGB, GL_UNSIGNED_BYTE, image->data);
	} else if (image->dataFormat == FormatRGBA) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					 image->width, image->height, 0,
					 GL_RGBA, GL_UNSIGNED_BYTE, image->data);
	}

	deleteImage(image);
}

void initGL() {
	testErrors("start of the function");

	glGenTextures(1, &nullTexture);

	/*
    InternalImage* image;
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    image = imageLoadTGA("images/Wood256.tga");

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->data);

    // TODO: free old image
    image = imageLoadTGA("images/LeavesDrawn512.tga");

    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
              image->width, image->height, 0,
              GL_RGBA, GL_UNSIGNED_BYTE, image->data);
	*/


    glClearColor(0.29, 0.55, 0.57, 1.0);

	testErrors("init color");

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
    glAlphaFunc(GL_GREATER, 0.001);
    glEnable(GL_ALPHA_TEST);

	//glEnable(GL_MULTISAMPLE);

	testErrors("init settings");

    //GLfloat lightPosition[] = {-8, 4, 6, 1};
    GLfloat lightAmbient[]  = {0.1, 0.1, 0.2, 0};
    //GLfloat lightDiffuse[]  = {0.8, 0.8, 0.8, 0};
	GLfloat lightDiffuse[]  = {1.0, 1.0, 1.0, 0};
    GLfloat lightSpecular[] = {0.4, 0.4, 0.4, 0};

	/* 1 / (const + (linear * dist(light, vertex) + quadratic * (dist(light, vertex))^2 )) */
	GLfloat lightConstantAttenuation = 1;
	GLfloat lightLinearAttenuation = 0;
	GLfloat lightQuadraticAttenuation = 0;

    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, lightConstantAttenuation);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, lightLinearAttenuation);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lightQuadraticAttenuation);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.fov, aspectRatio, camera.clipNear, camera.clipFar);

	testErrors("end of init");
}

void initWorld() {
    numberOfObjects = 1;
    objects = createObjectArray(numberOfObjects);

    Vec3f* tmpTranslation;
    Vec3f* tmpRotation;
    Vec3f* tmpScale;

    /* Sky sphere object */
    tmpTranslation = createVec3f(0, 0, 0);
    tmpRotation    = createVec3f(0, 0, 0);
    tmpScale       = createVec3f(10, 5, 10);
    objects[0] = createObject(tmpTranslation, tmpRotation, tmpScale, drawEmpty);

    /* Ground object */
    // setVec3f(tmpTranslation, 0, 0, 0);
    // setVec3f(tmpRotation,    0, 0, 0);
    // setVec3f(tmpScale,       1, 1, 1);
    // objects[1] = createObject(tmpTranslation, tmpRotation, tmpScale, drawGround);

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
    //testModel = loadModel("models/monkey.obj");
    //testModel = loadModel("models/test.obj");
	//tree = loadModel("models/tree_v2.obj");
	//skySphere = loadModel("models/skySphere.obj");
	levelMain  = loadModel("models/levelMain.obj");
    levelDecor = loadModel("models/levelDecor.obj");
    skyModel   = loadModel("models/skySphere.obj");

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
