#pragma once
#include "object.h"

class camera : public object
{
public:
    camera();

    void uniform_link(uniform& uni) override;

    void move(float dx, float dy, float dz);
    void move_forward(float distance);
    void move_backward(float distance);
    void move_left(float distance);
    void move_right(float distance);

    void add_rotate(float dalpha, float dbeta, float dgamma);

    float get_x() const { return dx; }
    float get_y() const { return dy; }
    float get_z() const { return dz; }
};