#pragma once
#include "engine/engine.h"

class object
{
public:
    virtual void set_rotate(float alpha, float beta, float gamma);
    virtual void set_translate(float dx, float dy, float dz);
    virtual void set_scale(float x, float y, float z);
    virtual void set_discretization(float, float);

    virtual void   calculate() {}
    virtual vertex get_vertex(double, double, rgba) { return vertex{}; }

    virtual void render(engine* _engine, camera& _camera);

    uint32_t size() const;
    triangle get_triangle(const uint32_t index);

    transformation_object get_transform() const { return tr_obj; }

protected:
    vertexes              _vertexes;
    std::vector<uint32_t> indexes;
    uint32_t              count_triangles = 0;

    // Step of discrete parameters
    float d1 = M_PI / 25;
    float d2 = M_PI / 25;

    transformation_object tr_obj;
};

using objects = std::vector<object>;