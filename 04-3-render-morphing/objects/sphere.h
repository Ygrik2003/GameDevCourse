#pragma once
#include "object.h"

class sphere : public object
{
public:
    sphere(float radius);

private:
    uint32_t get_index(int phi_c, int theta_c);
    vertex   get_vertex(double phi, double theta, rgba color) override;
    void     calculate() override;

    float radius;
};
