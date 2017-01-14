#include "camera.h"

void initCamera() {
    camera.position.x = -13.0;
    camera.position.y = 1.8;
    camera.position.z = -7.0;

    camera.rotation.x = 60.0;
    camera.rotation.y = 90.0;
    camera.rotation.z = 0.0;

    camera.fov      = 60.0;
    camera.clipNear = 0.01;
    camera.clipFar  = 200.0;
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
