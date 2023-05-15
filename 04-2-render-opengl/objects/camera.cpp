#include "camera.h"

void camera::set_rotate(float alpha, float beta, float gamma)
{
    tr_cam.rotate[0][0] = cos(alpha) * cos(beta);
    tr_cam.rotate[0][1] =
        cos(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma);
    tr_cam.rotate[0][2] =
        cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma);

    tr_cam.rotate[1][0] = sin(alpha) * cos(beta);
    tr_cam.rotate[1][1] =
        sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma);
    tr_cam.rotate[1][2] =
        sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma);

    tr_cam.rotate[2][0] = -sin(beta);
    tr_cam.rotate[2][1] = cos(beta) * sin(gamma);
    tr_cam.rotate[2][2] = cos(beta) * cos(gamma);

    tr_cam.rotate[3][3] = 1;
}

camera::camera(float front, float back, float fovy, float aspect)
{
    set_rotate(0., 0., 0.);
    set_translate(0., 0., 0.);
    set_scale(1., 1., 1.);

    tr_cam.projection[0][0] = 1 / (aspect * std::tan(fovy / 2));
    tr_cam.projection[1][1] = 1 / std::tan(fovy / 2);
    tr_cam.projection[2][2] = (back + front) / (back - front);
    tr_cam.projection[2][3] = -2 * back * front / (back - front);
    tr_cam.projection[3][2] = 1;
}

void camera::set_translate(float dx, float dy, float dz)
{
    tr_cam.translate[3][0] = dx;
    tr_cam.translate[3][1] = dy;
    tr_cam.translate[3][2] = dz;

    tr_cam.translate[0][0] = 1;
    tr_cam.translate[1][1] = 1;
    tr_cam.translate[2][2] = 1;
    tr_cam.translate[3][3] = 1;
}

void camera::set_scale(float x, float y, float z)
{
    tr_cam.scale[0][0] = x;
    tr_cam.scale[1][1] = y;
    tr_cam.scale[2][2] = z;
    tr_cam.scale[3][3] = 1;
}

void camera::move(float dx, float dy, float dz)
{
    pos.x += dx;
    pos.y += dy;
    pos.z += dz;

    set_translate(pos.x, pos.y, pos.z);
}

transformation_camera& camera::get_transform()
{
    return tr_cam;
}