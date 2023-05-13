#include "sphere_culinder.h"

#include <iostream>

sphere_culinder::sphere_culinder(float radius_sphere,
                                 float radius_culinder,
                                 float height_culinder)
{
    _sphere   = sphere(radius_sphere);
    _culinder = culinder(radius_culinder, height_culinder);
}

void sphere_culinder::render(engine* _engine, camera& _camera)
{
    for (uint32_t i = 0; i < size(); i++)
    {
        triangle tr_sphere   = _sphere.get_triangle(i);
        triangle tr_culinder = _culinder.get_triangle(i);
        std::cout << 1 << std::endl;
        _engine->render_triangle(
            tr_sphere, tr_culinder, alpha, tr_obj, _camera.get_transform());
    }
}