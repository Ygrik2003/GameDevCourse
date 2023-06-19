#pragma once
#include "engine/engine.h"
#include "objects/object.h"

#include <cstdint>
#include <vector>

class game
{
public:
    virtual void    add_object(object* obj)     = 0;
    virtual void    remove_object(size_t index) = 0;
    virtual object* get_object(size_t index)    = 0;

    virtual void render(camera) = 0;

    auto begin() { return _objects.begin(); }
    auto end() { return _objects.end(); }

protected:
    std::vector<object*> _objects;
    engine*              _engine;
};

class my_game : public game
{
public:
    my_game(engine* e);

    void    add_object(object* obj) override;
    void    remove_object(size_t index) override;
    object* get_object(size_t index) override;
    void    render(camera) override;
};
