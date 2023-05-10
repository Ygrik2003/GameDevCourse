#pragma once
#include "object.h"

class sphere : public object
{
public:
    sphere(float radius);

private:
    uint32_t get_index(int phi_c, int theta_c);
    vertex   get_sphere(double phi, double theta, uint32_t r);
    void     calculate() override;

    float radius;
};
