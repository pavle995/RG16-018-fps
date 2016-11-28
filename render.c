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

    root->rotation.y += 1;

    for (int i = 0; i < numberOfFlyingObjects; i++) {
        // flyingObjects[i]->rotation.x += randFloat(0, 2);
        // flyingObjects[i]->rotation.y += randFloat(0, 2);
        // flyingObjects[i]->rotation.z += randFloat(0, 2);

        flyingObjects[i]->rotation.x += 1;
        flyingObjects[i]->rotation.y += 1;
        flyingObjects[i]->rotation.z += 1;

        // flyingObjects[i]->translation.x += randFloat(-0.1, 0.1);
        // flyingObjects[i]->translation.y += randFloat(-0.1, 0.1);
        // flyingObjects[i]->translation.z += randFloat(-0.1, 0.1);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    calculateCamera();

    drawCoordinateSystem();

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
