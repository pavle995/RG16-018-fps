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

void displayScene() {
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
        GLfloat lightPosition[] = {-8, 4, 6, 1};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glPopMatrix();

    /* Draw test trees, TODO: put them in the scenegraph later */
    /* Tree 1 */
    glPushMatrix();
        glTranslatef(0.5, 0, -0.5);
        glRotatef(30, 0, 1, 0);
        drawModel(testModel, FULL);
    glPopMatrix();
    /* Tree 2 */
    glPushMatrix();
        glTranslatef(-2, 0, 2.5);
        glScalef(0.6, 0.6, 0.6);
        glRotatef(70, 0, 1, 0);
        drawModel(testModel, FULL);
    glPopMatrix();
    /* Tree 3 */
    glPushMatrix();
        glTranslatef(2.5, 0, 1.5);
        glScalef(0.4, 0.4, 0.4);
        glRotatef(130, 0, 1, 0);
        drawModel(testModel, FULL);
    glPopMatrix();

    glDisable(GL_LIGHTING);

    drawObjects();

    calculateInput();

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

void drawModel(Model model, ModelMode mode) {
    if (mode == POINTS) {
        /* Draw triangle points */
    	glPointSize(2);
    	for (int i = 0; i < model.numberOfVerticies; i++) {
    		glBegin(GL_POINTS);
    			glVertex3f(model.verticies[i].x, model.verticies[i].y, model.verticies[i].z);
    		glEnd();
    	}
    }

    else if (mode == WIRE) {
        /* Draw triangle lines */
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
    }

    else if (mode == FULL) {
        /* Draw triangle faces */
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable(GL_NORMALIZE); // TODO: fix scaling affecting normals

    	for (int i = 0; i < model.numberOfTriangles; i++) {
    	    glBegin(GL_TRIANGLES);

                if (model.triangles[i].smooth == 1)
                    glEnable(GL_SMOOTH);
                else
                    glDisable(GL_SMOOTH);

                /* Placeholder material colors (without lighting) */
                // if (model.numberOfMaterials > 0)
                //     glColor3f(model.materials[model.triangles[i].materialID].diffuse[0],
                //               model.materials[model.triangles[i].materialID].diffuse[1],
                //               model.materials[model.triangles[i].materialID].diffuse[2]);

                glMaterialfv(GL_FRONT, GL_AMBIENT,   model.materials[model.triangles[i].materialID].ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE,   model.materials[model.triangles[i].materialID].diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR,  model.materials[model.triangles[i].materialID].specular);
                glMaterialfv(GL_FRONT, GL_EMISSION,  model.materials[model.triangles[i].materialID].emission);
                glMaterialf( GL_FRONT, GL_SHININESS, model.materials[model.triangles[i].materialID].shininess);

                glNormal3f(model.triangles[i].vn0.x, model.triangles[i].vn0.y, model.triangles[i].vn0.z);
                glVertex3f(model.verticies[model.triangles[i].v0].x,
    					   model.verticies[model.triangles[i].v0].y,
    					   model.verticies[model.triangles[i].v0].z);

                glNormal3f(model.triangles[i].vn1.x, model.triangles[i].vn1.y, model.triangles[i].vn1.z);
                glVertex3f(model.verticies[model.triangles[i].v1].x,
    					   model.verticies[model.triangles[i].v1].y,
    					   model.verticies[model.triangles[i].v1].z);

                glNormal3f(model.triangles[i].vn2.x, model.triangles[i].vn2.y, model.triangles[i].vn2.z);
                glVertex3f(model.verticies[model.triangles[i].v2].x,
    					   model.verticies[model.triangles[i].v2].y,
    					   model.verticies[model.triangles[i].v2].z);
    	    glEnd();
    	}

        glDisable(GL_SMOOTH);
        glDisable(GL_CULL_FACE);
    }
}
