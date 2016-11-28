#include "input.h"

int key_w = 0;
int key_s = 0;
int key_a = 0;
int key_d = 0;
int key_up    = 0;
int key_down  = 0;
int key_left  = 0;
int key_right = 0;

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
