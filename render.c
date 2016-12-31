#include "render.h"

void resizeWindow(int width, int height) {
    windowHeight = height;
    windowWidth  = width;

    aspectRatio = calculateAspectRatio(windowWidth, windowHeight);

    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.fov, aspectRatio, camera.clipNear, camera.clipFar);
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
	dist = (1 / (1 - dist)) / 130;
	printf("dist: %f\n", dist);

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
	testErrors("before display scene");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (root != NULL)
        root->rotation.y += 1;

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

    drawCoordinateSystem();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // TODO: testing rotating light
    glPushMatrix();
        glRotatef(root->rotation.y + 80, 0, 1, 0);
		glTranslatef(-8, 4, 6);
        //GLfloat lightPosition[] = {-8, 4, 6, 1};
		//GLfloat lightPosition[] = {camera.position.x, camera.position.y, camera.position.z, 1};
		GLfloat lightPosition[] = {0, 0, 0, 1};


		//glTranslatef(camera.position.x, camera.position.y, camera.position.z);
		//glTranslatef(camera.position.x, 1, camera.position.z);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		glDisable(GL_LIGHTING);
		glColor3f(1, 1, 0.6);
		glutSolidSphere(0.4, 12, 8);
		glEnable(GL_LIGHTING);

    glPopMatrix();

    /* Draw test trees, TODO: put them in the scenegraph later */
    /* Tree 1 */
    glPushMatrix();
        //glTranslatef(0.5, 0, -0.5);
        //glRotatef(0, 0, 1, 0);
        drawModel(testModel, COMBINED);
    glPopMatrix();
    /* Tree 2 */
    glPushMatrix();
        glTranslatef(-2, -0.25, 2.5);
        glScalef(1, 1, 1);
        glRotatef(70, 0, 1, 0);
        drawModel(tree, COMBINED);
    glPopMatrix();
    /* Tree 3 */
    glPushMatrix();
        glTranslatef(-9, -0.5, -7.5);
        glScalef(1.2, 1.2, 1.2);
        glRotatef(130, 0, 1, 0);
        drawModel(tree, COMBINED);
    glPopMatrix();

    glDisable(GL_LIGHTING);

    drawObjects();

    calculateInput();

	/* Get aim hit */
	Vec3f* hit = aimHit();
	glPushMatrix();
		glTranslatef(hit->x, hit->y, hit->z);
		glColor3f(1, 0, 0);
		glutWireSphere(0.2, 12, 8);
	glPopMatrix();
	printf("hit cords: %f %f %f\n", hit->x, hit->y, hit->z);
	deleteVec3f(hit);

    glutSwapBuffers();
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
    drawSceneObjects(root);
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

    glEnable(GL_BLEND);
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

    glDisable(GL_BLEND);
		testErrors("gl Disable blend");
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

void drawModel(Model model, ModelMode mode) {
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
