#pragma once
#include "object.h"

class culinder : public object
{
public:
    culinder(float radius, float height);

private:
    uint32_t get_index(int phi_c, int theta_c);
    vertex   get_vertex(double phi, double height, rgba) override;
    void     calculate() override;

    float radius;
    float height;
};