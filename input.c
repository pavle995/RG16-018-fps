#include "input.h"

int key_w = 0;
int key_s = 0;
int key_a = 0;
int key_d = 0;
int key_up    = 0;
int key_down  = 0;
int key_left  = 0;
int key_right = 0;

int visualDebug = 0;

void mouseLook(int mouseX, int mouseY) {
    float sensitivity = 0.2;

	/* How much the mouse moved */
	int dx = mouseX - windowWidth  / 2;
	int dy = mouseY - windowHeight / 2;

	/* Add mouse sensitivity and fix aspect ratio diference */
    camera.rotation.x = camera.rotation.x + (dx * sensitivity);
    camera.rotation.y = camera.rotation.y + (dy * sensitivity * aspectRatio);
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

int getXCell(float x) {
    float halfWidth = (float)levelGrid->width  / 2;
    int   result;

    if (levelGrid->width % 2 == 0) {
        result = floorf(halfWidth  + x + 0.5);
    } else {
        result = floorf(halfWidth + x);
    }

    return result;
}

int getZCell(float z) {
    float halfHeight = (float)levelGrid->height / 2;
    int   result;

    if (levelGrid->height % 2 == 0) {
        result = floorf(halfHeight + z + 0.5);
    } else {
        result = floorf(halfHeight + z);
    }

    return result;
}

int noWallCollision(float x, float y, float z) {
    UNUSED(y);

    int testX = getXCell(x);
    int testZ = getZCell(z);

    /* Check bounds */
    if (testX < 0)
        testX = 0;
    else if (testX > levelGrid->width - 1)
        testX = levelGrid->width - 1;

    if (testZ < 0)
        testZ = 0;
    else if (testZ > levelGrid->height - 1)
        testZ = levelGrid->height -1 ;

    /*
    for (int i = 0; i < levelGrid->height; i++) {
        for (int j = 0; j < levelGrid->width; j++) {
            if (i == testZ && j == testX) {
                printf("@");
            } else {
                if ((levelGrid->data)[i][j] == blockedCell)
                    printf("#");
                else
                    printf(" ");
            }
        }
        printf("\n");
    }
    */
    //printf("Position      : %.2f %.2f\n", x, z);
    //printf("Collision test: %d %d\n\n", testX, testZ);


    if (visualDebug) {
        glPushMatrix();
            glColor3f(0, 1, 1);
            glTranslatef(testX - levelGrid->width / 2, 0.5, testZ  - levelGrid->height / 2);
            glutWireSphere(0.4, 8, 6);
        glPopMatrix();
    }


    /* Check collision in the grid */
    if ((levelGrid->data)[testZ][testX] == emptyCell)
        return 1;
    else if ((levelGrid->data)[testZ][testX] == blockedCell)
        return 0;
    else
        return 1;
}

typedef enum {
    upLeftPos = 0, upPos,     upRightPos,
    leftPos,       centerPos, rightPos,
    downLeftPos,   downPos,   downRightPos
} GridPositions;

void checkCollisionMove(float fromX, float fromZ,
                        float toX,   float toZ) {
    float newX = fromX;
    float newZ = fromZ;

    /* Check collision place */
    int startX = getXCell(fromX);
    int startZ = getZCell(fromZ);

    int endX = getXCell(toX);
    int endZ = getZCell(toZ);

    int pos = centerPos;

    /* Center */
    if ((endX == startX) && (endZ == startZ))
        pos = centerPos;
    /* Left */
    else if (((endX - startX) == -1) && (endZ == startZ))
        pos = leftPos;
    /* Right */
    else if (((endX - startX) == 1) && (endZ == startZ))
        pos = rightPos;
    /* Up */
    else if ((endZ - startZ) == -1) {
        /* Up Left */
        if ((endX - startX) == -1)
            pos = upLeftPos;
        /* Up */
        else if (endX == startX)
            pos = upPos;
        /* Up Right */
        else if ((endX - startX) == 1)
            pos = upRightPos;
    }
    /* Down */
    else if ((endZ - startZ) == 1) {
        /* Down Left */
        if ((endX - startX) == -1)
            pos = downLeftPos;
        /* Down */
        else if (endX == startX)
            pos = downPos;
        /* Down Right */
        else if ((endX - startX) == 1)
            pos = downRightPos;
    }

    if (pos == upLeftPos)
        pos = upPos;
    else if (pos == upRightPos)
        pos = upPos;
    else if (pos == downLeftPos)
        pos = downPos;
    else if (pos == downRightPos)
        pos = downPos;

    //float rotation = 360 - camera.rotation.x;

    if (noWallCollision(toX, 0, toZ)) {
        newX = toX;
        newZ = toZ;
    }

    /* Center position */
    else if (pos == centerPos) {
        newX = toX;
        newZ = toZ;
    }
    /* Left position */
    else if (pos == leftPos) {
        if (noWallCollision(fromX, 0, toZ))
            newZ = toZ;
    }
    /* Right position */
    else if (pos == rightPos) {
        if (noWallCollision(fromX, 0, toZ))
            newZ = toZ;
    }
    /* Up left position */
    else if (pos == upLeftPos) {
        if (noWallCollision(toX, 0, fromZ))
            newX = toX;
        /*
        if (rotation >= 135) {
            if (noWallCollision(toX, 0, fromZ))
                newX = toX;
        }
        else if (rotation < 135) {
            if (noWallCollision(fromX, 0, toZ))
                newZ = toZ;
        }
        */
    }
    /* Up position */
    else if (pos == upPos) {
        if (noWallCollision(toX, 0, fromZ))
            newX = toX;
    }
    /* Up right position */
    else if (pos == upRightPos) {
        //if (noWallCollision(toX, 0, fromZ))
            //newX = toX;
        if (noWallCollision(fromX, 0, toZ))
            newZ = toZ;
        /*
        if (rotation >= 45) {
            if (noWallCollision(fromX, 0, toZ))
                newZ = toZ;
        }
        else if (rotation < 45) {
            if (noWallCollision(toX, 0, fromZ))
                newX = toX;
        }
        */
    }
    /* Down left position */
    else if (pos == downLeftPos) {
        if (noWallCollision(toX, 0, fromZ))
            newX = toX;
        /*
        if (rotation >= 225) {
            if (noWallCollision(fromX, 0, toZ))
                newZ = toZ;
        }
        else if (rotation < 225) {
            if (noWallCollision(toX, 0, fromZ))
                newX = toX;
        }
        */
    }
    /* Down position */
    else if (pos == downPos) {
        if (noWallCollision(toX, 0, fromZ))
            newX = toX;
    }
    /* Down right position */
    else if (pos == downRightPos) {
        //if (noWallCollision(toX, 0, fromZ))
            //newX = toX;
        if (noWallCollision(fromX, 0, toZ))
            newZ = toZ;
        /*
        if (rotation >= 315) {
            if (noWallCollision(toX, 0, fromZ))
                newX = toX;
        }
        else if (rotation < 315) {
            if (noWallCollision(fromX, 0, toZ))
                newZ = toZ;
        }
        */
    }

    if (visualDebug) {
        glPushMatrix();
            glColor3f(0, 0, 1);
            glTranslatef(toX, 0.5, toZ);
            glutWireSphere(0.4, 8, 6);
        glPopMatrix();
    }

    /*
    printf("%4.0f  ", camera.rotation.x);
    printf("%4.0f  ", rotation);
    if (pos == upLeftPos)
        printf("pos: upLeftPos\n");
    else if (pos == upPos)
        printf("pos: upPos\n");
    else if (pos == upRightPos)
        printf("pos: upRightPos\n");
    else if (pos == leftPos)
        printf("pos: leftPos\n");
    else if (pos == centerPos)
        printf("pos: centerPos\n");
    else if (pos == rightPos)
        printf("pos: rightPos\n");
    else if (pos == downLeftPos)
        printf("pos: downLeftPos\n");
    else if (pos == downPos)
        printf("pos: downPos\n");
    else if (pos == downRightPos)
        printf("pos: downRightPos\n");
    */

    camera.position.x = newX;
    camera.position.z = newZ;
}

void calculateInput() {
    float step = 0.0035 * deltaTime;
    float turn = 0.15 * deltaTime;

    Vec3f move;

    /* Move camera forward */
    if (key_w == 1 && key_s == 0) {
        move.x = camera.position.x + camera.look.x * step;
        move.y = camera.position.y + camera.look.y * step;
        move.z = camera.position.z + camera.look.z * step;
        checkCollisionMove(camera.position.x, camera.position.z, move.x, move.z);
    }

    /* Move camera backward */
    if (key_s == 1 && key_w == 0) {
        move.x = camera.position.x - camera.look.x * step;
        move.y = camera.position.y - camera.look.y * step;
        move.z = camera.position.z - camera.look.z * step;
        checkCollisionMove(camera.position.x, camera.position.z, move.x, move.z);
    }

    /* Move camera to the left side */
    if (key_a == 1 && key_d == 0) {
        move.x = camera.position.x + camera.look.z * step;
        move.y = camera.position.y;
        move.z = camera.position.z - camera.look.x * step;
        checkCollisionMove(camera.position.x, camera.position.z, move.x, move.z);
    }

    /* Move camera to the right side */
    if (key_d == 1 && key_a == 0) {
        move.x = camera.position.x - camera.look.z * step;
        move.y = camera.position.y;
        move.z = camera.position.z + camera.look.x * step;
        checkCollisionMove(camera.position.x, camera.position.z, move.x, move.z);
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

void keyboardBasicPress(unsigned char key, int mouseX, int mouseY) {
    UNUSED(mouseX);
    UNUSED(mouseY);

    int convert_key = key;      /* Fix multichar UTF-8 character overflow conversion */

    switch (convert_key) {
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

        case 'h':
            if (visualDebug == 0)
                visualDebug = 1;
            else
                visualDebug = 0;
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
    UNUSED(mouseX);
    UNUSED(mouseY);

    int convert_key = key;      /* Fix multichar UTF-8 character overflow conversion */

    switch (convert_key) {
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
    UNUSED(mouseX);
    UNUSED(mouseY);

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
    UNUSED(mouseX);
    UNUSED(mouseY);

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
