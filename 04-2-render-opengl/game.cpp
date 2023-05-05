#include "game.h"
#include <cassert>

sphere::sphere(int radius) {}

void sphere::set_rotate(double alpha, double beta, double gamma) {}
void sphere::set_translate(double dx, double dy, double dz) {}
void sphere::set_scale(double scale_x, double scale_y, double scale_z) {}
void sphere::set_discretization(double d_phi, double d_theta)
{
    d1 = d_phi;
    d2 = d_theta;
}

sphere_game::sphere_game(engine& e) {}

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