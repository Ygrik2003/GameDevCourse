#include "game.h"
#include <cassert>

sphere::sphere(int radius) {}

void object::set_rotate(float alpha, float beta, float gamma) {}
void object::set_translate(float dx, float dy, float dz) {}
void object::set_scale(float scale_x, float scale_y, float scale_z) {}
void object::set_discretization(float d_phi, float d_theta)
{
    d1 = d_phi;
    d2 = d_theta;
}

sphere_game::sphere_game(engine* e) {}

void sphere_game::add_object(const object& obj)
{
    objects.push_back(obj);
}
void sphere_game::remove_object(size_t index)
{
    assert(index < objects.size());
    objects.erase(objects.begin() + index);
}
object sphere_game::get_object(size_t index)
{
    assert(index < objects.size());
    return objects[index];
}