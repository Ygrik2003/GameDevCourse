#pragma once
#include "culinder.h"
#include "sphere.h"

class sphere_culinder : public object
{
public:
    sphere_culinder(float radius_sphere,
                    float radius_culinder,
                    float height_culinder);

    void render(engine* _engine, camera& _camera) override;

protected:
    vertexes sub_vertexes;

private:
    float radius_sphere;
    float radius_culinder;
    float height_culinder;

    object _sphere;
    object _culinder;

    float alpha = 0.f;
};