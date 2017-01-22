#include "render.h"

int  numberOfFrames = 0;
long lastTime = 0;
long lastFpsTime = 0;
int  framesPerSecondDisplay = 0;

void resizeWindow(int width, int height) {
    windowHeight = height;
    windowWidth  = width;

    aspectRatio = calculateAspectRatio(windowWidth, windowHeight);

    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.fov, aspectRatio, camera.clipNear, camera.clipFar);
}

int limitFps(float fpsLimit) {
    if (fpsLimit == 0)
        return 0;
    if (deltaTime < (1000.0 / fpsLimit))
        return 1;
    return 0;
}

Vec3f* aimHit() {
	Vec3f* hit = createVec3f(0, 0, 0);

	float dist;

	float distX;
	float distY;
	float distZ;

	glReadPixels(windowWidth / 2, windowHeight / 2,
					1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dist);
    /* TODO: Distance is aproximated, a very bad thing, FIX THIS!!! */
    /* FIXME: Find another way to do this (selection buffer render??) */
	dist = (1 / (1 - dist)) / 130; /* TODO: It's not a linear function */
	//printf("dist: %f\n", dist);

	distX = camera.look.x * dist;
	distY = camera.look.y * dist;
	distZ = camera.look.z * dist;

	// WRONG - gets the middle pixel coordinates as if the screen was in the virtual space, but ray hit is required
	GLdouble modelviewMatrix[16];
	GLdouble projectionMatrix[16];
	GLint    viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX,  modelviewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
	glGetIntegerv(GL_VIEWPORT,         viewport);

	GLdouble x;
	GLdouble y;
	GLdouble z;

	gluUnProject(((float) windowWidth)  / 2,		// Center of the window
				 ((float) windowHeight) / 2,
				  0,
				  modelviewMatrix,
				  projectionMatrix,
				  viewport,
				  &(x),
				  &(y),
				  &(z));


	hit->x = (float) x + distX;
	hit->y = (float) y + distY;
	hit->z = (float) z + distZ;

	return hit;
}

void displayScene() {
	//testErrors("before display scene");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Calculate time */
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime   = currentTime - lastTime;
    /* Limit fps (if needed) */
    if (limitFps(0)) {
        return;
    }
    lastTime    = currentTime;

    /* Calculate frames per second */
	numberOfFrames++;
	if (currentTime - lastFpsTime > 1000) {
        float framesPerSecond = numberOfFrames * 1000.0 / (currentTime - lastFpsTime);
        framesPerSecondDisplay = framesPerSecond;
        lastFpsTime = currentTime;
		numberOfFrames = 0;
	}

    /* TODO: Remove this old scene debug rotation */
    if (root != NULL)
        root->rotation.y += (0.05 * deltaTime);

    // for (int i = 0; i < numberOfFlyingObjects; i++) {
    //     flyingObjects[i]->rotation.x += randFloat(0, 2);
    //     flyingObjects[i]->rotation.y += randFloat(0, 2);
    //     flyingObjects[i]->rotation.z += randFloat(0, 2);
    //
    //     flyingObjects[i]->rotation.x += 1;
    //     flyingObjects[i]->rotation.y += 1;
    //     flyingObjects[i]->rotation.z += 1;
    //
    //     flyingObjects[i]->translation.x += randFloat(-0.1, 0.1);
    //     flyingObjects[i]->translation.y += randFloat(-0.1, 0.1);
    //     flyingObjects[i]->translation.z += randFloat(-0.1, 0.1);
    // }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    calculateCamera();

    glPushMatrix();
        glTranslatef(camera.position.x, camera.position.y - 5.0, camera.position.z);
        glRotatef(90, 1, 0, 0);
        glScalef(10.0, 5.0, 10.0);
        glColor3f(1.0, 1.0, 1.0);
        drawDisplayListModel(skyModel);
    glPopMatrix();
    glClear(GL_DEPTH_BUFFER_BIT);

    if (visualDebug) {
        drawCoordinateSystem();
        drawLevelGrid(levelGrid);
        //drawLevelGraph(levelGraph, levelGrid);
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // TODO: testing rotating light
    // glPushMatrix();
    //     glRotatef(root->rotation.y + 80, 0, 1, 0);
	// 	glTranslatef(-8, 4, 6);
    //     //GLfloat lightPosition[] = {-8, 4, 6, 1};
	// 	//GLfloat lightPosition[] = {camera.position.x, camera.position.y, camera.position.z, 1};
	// 	GLfloat lightPosition[] = {0, 0, 0, 1};
    //
    //
	// 	//glTranslatef(camera.position.x, camera.position.y, camera.position.z);
	// 	//glTranslatef(camera.position.x, 1, camera.position.z);
	// 	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    //
	// 	glDisable(GL_LIGHTING);
	// 	glColor3f(1, 1, 0.6);
	// 	glutSolidSphere(0.4, 12, 8);
	// 	glEnable(GL_LIGHTING);
    //
    // glPopMatrix();

    /* Draw test trees, TODO: put them in the scenegraph later */
    // glPushMatrix();
    //     //glTranslatef(0.5, 0, -0.5);
    //     //glRotatef(0, 0, 1, 0);
    //     drawModel(testModel, FULL);
    // glPopMatrix();
    // /* Tree 1 */
    // glPushMatrix();
    //     glTranslatef(-2, -0.25, 2.5);
    //     glScalef(1, 1, 1);
    //     glRotatef(70, 0, 1, 0);
    //     drawModel(tree, FULL);
    // glPopMatrix();
    // /* Tree 2 */
    // glPushMatrix();
    //     glTranslatef(-9, -0.5, -7.5);
    //     glScalef(1.2, 1.2, 1.2);
    //     glRotatef(130, 0, 1, 0);
    //     drawModel(tree, FULL);
    // glPopMatrix();

    /* TODO: Stop the camera from cliping through the walls */

    /* Draw level models */
    glPushMatrix();
        glTranslatef(-1.5, 0, -2.5);
        drawDisplayListModel(levelMain);
        drawDisplayListModel(levelDecor);
    glPopMatrix();

    glDisable(GL_LIGHTING);

    drawObjects();

	/* Get aim hit */
	// Vec3f* hit = aimHit();  /* TODO: for some reason it glitches the mouse look */
	// glPushMatrix();
	// 	glTranslatef(hit->x, hit->y, hit->z);
	// 	glColor3f(1, 0, 0);
	// 	glutWireSphere(0.2, 12, 8);
	// glPopMatrix();
	// //printf("hit cords: %f %f %f\n", hit->x, hit->y, hit->z);
	// deleteVec3f(hit);

    /* Draw 2D HUD */
    setDrawing2D();

    /* Draw the crosshair TODO: Put this in a separate function */
    /* Crosshair Shadow */
	glLineWidth(4);
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_LINES);
		/* Up */
		glVertex2f(0, 0.017);
		glVertex2f(0, 0.053);

		/* Down */
		glVertex2f(0, -0.017);
		glVertex2f(0, -0.053);

		/* Left */
		glVertex2f(-0.017, 0);
		glVertex2f(-0.053 / aspectRatio, 0);

		/* Right */
		glVertex2f( 0.017, 0);
	    glVertex2f( 0.053 / aspectRatio, 0);
	glEnd();
	glLineWidth(1);

	/* Crosshair */
	glLineWidth(2);
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
		/* Up */
		glVertex2f(0, 0.02);
		glVertex2f(0, 0.05);

		/* Down */
		glVertex2f(0, -0.02);
		glVertex2f(0, -0.05);

		/* Left */
		glVertex2f(-0.02, 0);
		glVertex2f(-0.05 / aspectRatio, 0);

		/* Right */
		glVertex2f( 0.02,  0);
		glVertex2f( 0.05 / aspectRatio,  0);
	glEnd();


    /* Draw text on the screen */
    setDrawing2DText();
    glColor3f(0.8, 0.8, 0.3);

    char tmpString[100];

    sprintf(tmpString, "FPS: %d", framesPerSecondDisplay);
    drawTextLine(10, 10, tmpString);
    drawTextLine(10, 40, "Press");
    glColor3f(0.8, 0.2, 0.2);
    drawTextLine(62, 40, "h");
    glColor3f(0.8, 0.8, 0.3);
    drawTextLine(78, 40, "for debug details");

    if (visualDebug) {
        /* Draw a transparent background quad */
        glEnable(GL_BLEND);
            glColor4f(0.1, 0.1, 0.3, 0.4);
            glRecti(45, 75, 430, 200);
        glDisable(GL_BLEND);
        glColor3f(0.8, 0.8, 0.3);

        /* Write debug information */
        sprintf(tmpString, "Camera position:  (%5.2f    %5.2f    %5.2f)", camera.position.x, camera.position.y, camera.position.z);
        drawTextLine(50, 80, tmpString);
        sprintf(tmpString, "Camera rotation:  (%5.2f    %5.2f    %5.2f)", camera.rotation.x, camera.rotation.y, camera.rotation.z);
        drawTextLine(50, 105, tmpString);
        sprintf(tmpString, "Camera look:        (%5.2f    %5.2f    %5.2f)", camera.look.x, camera.look.y, camera.look.z);
        drawTextLine(50, 130, tmpString);

        int currentX = getXCell(camera.position.x);
        int currentZ = getZCell(camera.position.z);
        sprintf(tmpString, "Grid location:  (%d  %d)", currentX, currentZ);
        drawTextLine(50, 170, tmpString);
    }

    /* Return to the 3D */
    setDrawing3D();

    calculateInput();

    glFlush();
    //glutSwapBuffers();
}

void drawObjects() {
    for (int i = 0; i < numberOfObjects; i++) {
        glPushMatrix();
            glTranslatef(objects[i]->translation.x,
                         objects[i]->translation.y,
                         objects[i]->translation.z);

            glRotatef(objects[i]->rotation.x, 1, 0, 0);
            glRotatef(objects[i]->rotation.y, 0, 1, 0);
            glRotatef(objects[i]->rotation.z, 0, 0, 1);

            glScalef(objects[i]->scale.x,
                     objects[i]->scale.y,
                     objects[i]->scale.z);

            objects[i]->drawFunction();
        glPopMatrix();
    }

    /* Draw scene graph */
    //drawSceneObjects(root);
}

void drawSceneObjects(Object* object) {
    if (object == NULL)
        return;

    glPushMatrix();
        glRotatef(object->rotation.x, 1, 0, 0);
        glRotatef(object->rotation.y, 0, 1, 0);
        glRotatef(object->rotation.z, 0, 0, 1);

        glTranslatef(object->translation.x,
                     object->translation.y,
                     object->translation.z);

        glScalef(object->scale.x,
                 object->scale.y,
                 object->scale.z);

        object->drawFunction();

        for (int i = 0; i < object->numberOfChildren; i++) {
            drawSceneObjects(object->children[i]);
        }

    glPopMatrix();
}

void drawModelPoints(Model model) {
	glDisable(GL_LIGHTING);

	glPointSize(5);
	for (int i = 0; i < model.numberOfVerticies; i++) {
		glBegin(GL_POINTS);
			glVertex3f(model.verticies[i].x, model.verticies[i].y, model.verticies[i].z);
		glEnd();
	}

	glEnable(GL_LIGHTING);
}

void drawModelWireframe(Model model) {
	glDisable(GL_LIGHTING);

	glLineWidth(2);
	for (int i = 0; i < model.numberOfTriangles; i++) {
		glBegin(GL_LINES);
            /* Placeholder material colors (without lighting) TODO: fix or remove wireframe colors */
            if (model.numberOfMaterials > 0)
                glColor3f(model.materials[model.triangles[i].materialID].diffuse[0],
                          model.materials[model.triangles[i].materialID].diffuse[1],
                          model.materials[model.triangles[i].materialID].diffuse[2]);

			/* 1st line */
			glVertex3f(model.verticies[model.triangles[i].v0].x,
					   model.verticies[model.triangles[i].v0].y,
				   	   model.verticies[model.triangles[i].v0].z);
		    glVertex3f(model.verticies[model.triangles[i].v1].x,
					   model.verticies[model.triangles[i].v1].y,
				   	   model.verticies[model.triangles[i].v1].z);

			/* 2nd line */
		    glVertex3f(model.verticies[model.triangles[i].v0].x,
					   model.verticies[model.triangles[i].v0].y,
				   	   model.verticies[model.triangles[i].v0].z);
		    glVertex3f(model.verticies[model.triangles[i].v2].x,
					   model.verticies[model.triangles[i].v2].y,
				   	   model.verticies[model.triangles[i].v2].z);

			/* 3rd line */
		    glVertex3f(model.verticies[model.triangles[i].v1].x,
					   model.verticies[model.triangles[i].v1].y,
				   	   model.verticies[model.triangles[i].v1].z);
		    glVertex3f(model.verticies[model.triangles[i].v2].x,
					   model.verticies[model.triangles[i].v2].y,
				   	   model.verticies[model.triangles[i].v2].z);
		glEnd();
	}

	glEnable(GL_LIGHTING);
}

void drawModelFull(Model model) {
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_RESCALE_NORMAL); // TODO: fix scaling affecting normals

    //glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	testErrors("gl enables");

	for (int i = 0; i < model.numberOfTriangles; i++) {
		//if (model.triangles[i].smooth == 1)		/* TODO: add sharp edges, planar triangles should be smooth */
        //	glShadeModel(GL_SMOOTH);
       	//else
        //	glShadeModel(GL_FLAT);
		glShadeModel(GL_SMOOTH);
		testErrors("gl shade model gl smooth");

		if (model.numberOfMaterials > 0) {
			if (model.materials[model.triangles[i].materialID].textureID != nullTexture) {
            	glBindTexture(GL_TEXTURE_2D, model.materials[model.triangles[i].materialID].textureID);
				//glBindTexture(GL_TEXTURE_2D, 2);

			}
			testErrors("gl bind texture");

            // Placeholder material colors (without lighting)
            //if (model.numberOfMaterials > 0)
            //	glColor3f(model.materials[model.triangles[i].materialID].diffuse[0],
            //              model.materials[model.triangles[i].materialID].diffuse[1],
            //              model.materials[model.triangles[i].materialID].diffuse[2]);

            glMaterialfv(GL_FRONT, GL_AMBIENT,   model.materials[model.triangles[i].materialID].ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE,   model.materials[model.triangles[i].materialID].diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR,  model.materials[model.triangles[i].materialID].specular);
            glMaterialfv(GL_FRONT, GL_EMISSION,  model.materials[model.triangles[i].materialID].emission);
            //glMaterialf( GL_FRONT, GL_ALPHA,     model.materials[model.triangles[i].materialID].alpha);	// WRONG!!
            glMaterialf( GL_FRONT, GL_SHININESS, model.materials[model.triangles[i].materialID].shininess);
			testErrors("gl material");

			//printf("MaterialID: %d,  TextureID: %d\n",
			//										model.triangles[i].materialID,
			//										model.materials[model.triangles[i].materialID].textureID);

		}

	    glBegin(GL_TRIANGLES);
            glNormal3f(model.triangles[i].vn0.x, model.triangles[i].vn0.y, model.triangles[i].vn0.z);
            glTexCoord2f(model.triangles[i].texcoord0.x, model.triangles[i].texcoord0.y);
            glVertex3f(model.verticies[model.triangles[i].v0].x,
					   model.verticies[model.triangles[i].v0].y,
					   model.verticies[model.triangles[i].v0].z);

            glNormal3f(model.triangles[i].vn1.x, model.triangles[i].vn1.y, model.triangles[i].vn1.z);
            glTexCoord2f(model.triangles[i].texcoord1.x, model.triangles[i].texcoord1.y);
            glVertex3f(model.verticies[model.triangles[i].v1].x,
					   model.verticies[model.triangles[i].v1].y,
					   model.verticies[model.triangles[i].v1].z);

            glNormal3f(model.triangles[i].vn2.x, model.triangles[i].vn2.y, model.triangles[i].vn2.z);
            glTexCoord2f(model.triangles[i].texcoord2.x, model.triangles[i].texcoord2.y);
            glVertex3f(model.verticies[model.triangles[i].v2].x,
					   model.verticies[model.triangles[i].v2].y,
					   model.verticies[model.triangles[i].v2].z);
        glEnd();
			testErrors("gl end");
	}

    //glDisable(GL_BLEND);
	//	testErrors("gl Disable blend");
    glDisable(GL_TEXTURE_2D);
		testErrors("gl Disable texture 2D");

    glDisable(GL_RESCALE_NORMAL); // TODO: fix scaling affecting normals
		testErrors("gl Disable rescale normal");

    glShadeModel(GL_SMOOTH);
		testErrors("gl shade model gl smooth");
    glDisable(GL_CULL_FACE);
		testErrors("gl Disable cull face");

	testErrors("gl Disables");
}

void drawModelCombined(Model model) {
	drawModelFull(model);
	drawModelPoints(model);
	drawModelWireframe(model);
}

void drawRawModel(Model model, ModelMode mode) {
    if (mode == POINTS) {
        /* Draw triangle points */
		drawModelPoints(model);
    }

    else if (mode == WIRE) {
        /* Draw triangle lines */
		drawModelWireframe(model);
    }

    else if (mode == FULL) {
        /* Draw triangle faces */
        drawModelFull(model);
    }

	else if (mode == COMBINED) {
		/* Draw model shaded with points and wireframe */
		drawModelCombined(model);
	}
	/* TODO: else error check unknown enum */
}

void drawDisplayListModel(Model model) {
    glCallList(model.displayListID);
}

void setDrawing2D() {
    /* Save 3D projection matrix */
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
        /* Save 3D modelview matrix */
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
            /* Stop using z-buffer (2D HUD/GUI is over other 3D objects) */
            glDisable(GL_DEPTH_TEST);
}

void setDrawing2DText() {
            /* Set the projection matrix to be easily usable by direct pixel coordinates */
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0, windowWidth, windowHeight, 0);
            /* Clear transformations */
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
}

void setDrawing3D() {
            /* Use the z-buffer again for 3D objects */
            glEnable(GL_DEPTH_TEST);
        /* Restore old 3D modelview matrix */
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    /* Restore old 3D projection matrix */
    glPopMatrix();
    /* Set the modelview matrix as the active matrix */
    glMatrixMode(GL_MODELVIEW);
}

void drawText(float x, float y, const char* string, int lineLength) {
    char* c;
    int offsetY = 18;   /* Fixed size by the used GLUT font */
    glRasterPos3f(x, y + offsetY, 0);
    int newLinePos = 0;
    int currentPos = 0;
    /* Draw lines with maximum length (and then go to the new line) */
    if (lineLength > 0) {
        for (c = (char*)string; *c != '\0'; c++) {
            if ((*c == '\n') || (currentPos == lineLength)) {
                newLinePos += 20;
                glRasterPos3f(x, y + offsetY + newLinePos, 0);
                currentPos = 0;
            }
            /* Don't draw newline character (it would be drawn as a ' ' character)*/
            if (*c != '\n')
                glutBitmapCharacter(font, *c);
            currentPos++;
        }
    }
    /* If the line length is 0, there is no line length limit */
    else if (lineLength == 0) {
        for (c = (char*)string; *c != '\0'; c++) {
            if (*c == '\n') {
                newLinePos += 20;
                glRasterPos3f(x, y + offsetY + newLinePos, 0);
            }
            if (*c != '\n')
                glutBitmapCharacter(font, *c);
        }
    }
}

void drawTextLines(float x, float y, const char* string) {
    drawText(x, y, string, 0);
}

void drawTextLine(float x, float y, const char* string) {
    char* c;
    int offsetY = 18;
    glRasterPos3f(x, y + offsetY, 0);
    for (c = (char*)string; *c != '\0'; c++) {
        if (*c != '\n')
            glutBitmapCharacter(font, *c);
    }
}
