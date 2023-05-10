#include "game.h"
#include <cassert>

#include <iostream>

sphere_game::sphere_game(engine* e)
{
    _engine = e;
}

void sphere_game::add_object(const object& obj)
{
    _objects.push_back(obj);
}

void sphere_game::remove_object(size_t index)
{
    assert(index < _objects.size());
    _objects.erase(_objects.begin() + index);
}

object& sphere_game::get_object(size_t index)
{
    assert(index < _objects.size());
    return _objects[index];
}
