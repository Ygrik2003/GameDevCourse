#include "camera.h"
#include "iostream"

camera::camera()
{
    set_rotate(0, 0, 0);
    set_translate(0, 0, 0);
    set_scale(1, 1, 1);
}

void camera::uniform_link(uniform& uni)
{
    uni.rotate_alpha_camera = &alpha;
    uni.rotate_beta_camera  = &beta;
    uni.rotate_gamma_camera = &gamma;

    uni.translate_x_camera = &dx;
    uni.translate_y_camera = &dy;
    uni.translate_z_camera = &dz;
}

void camera::move(float dx, float dy, float dz)
{
    this->dx += dx;
    this->dy += dy;
    this->dz += dz;
}

void camera::move_forward(float distance)
{
    // move(0, 0, distance);
    move(distance * std::cos(gamma) * std::sin(beta),
         distance * std::sin(beta),
         -distance * std::cos(gamma) * std::cos(beta));
    // move(-distance * std::sin(alpha) * std::cos(beta),
    //      distance * std::sin(beta),
    //      -distance * std::cos(alpha) * std::cos(beta));
}

void camera::move_backward(float distance)
{
    // move(0, 0, -distance);
    move(-distance * std::cos(gamma) * std::sin(beta),
         -distance * std::sin(beta),
         distance * std::cos(gamma) * std::cos(beta));
    // move(distance * std::sin(alpha) * std::cos(beta),
    //      -distance * std::sin(beta),
    //      distance * std::cos(alpha) * std::cos(beta));
}

void camera::move_left(float distance)
{
    move(-distance * std::cos(gamma), 0, -distance * std::sin(gamma));
    // move(distance * std::cos(alpha), 0, -distance * std::sin(alpha));
}

void camera::move_right(float distance)
{
    move(distance * std::cos(gamma), 0, distance * std::sin(gamma));
    // move(-distance * std::cos(alpha), 0, distance * std::sin(alpha));
}

void camera::add_rotate(float dalpha, float dbeta, float dgamma)
{
    alpha += dalpha;
    beta += dbeta;
    gamma += dgamma;
    std::cout << alpha << " " << beta << " " << gamma << std::endl;
}
