#include "game.h"

#include <cassert>
#include <iostream>

my_game::my_game(engine* e)
{
    _engine = e;
}

void my_game::add_object(object* obj)
{
    _objects.push_back(obj);
}

void my_game::remove_object(size_t index)
{
    assert(index < _objects.size());
    _objects.erase(_objects.begin() + index);
}

object* my_game::get_object(size_t index)
{
    assert(index < _objects.size());
    return _objects[index];
}

void my_game::render(camera _camera)
{
    for (auto obj : _objects)
    {
        obj->render(_engine, _camera);
    }
}