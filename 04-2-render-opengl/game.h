#include "engine/engine.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <vector>

class object
{
public:
    virtual void set_rotate(float alpha, float beta, float gamma);
    virtual void set_translate(float dx, float dy, float dz);
    virtual void set_scale(float x, float y, float z);
    virtual void set_discretization(float, float);

protected:
    vertexes vertexes_object;

    // Step of discrete parameters
    float d1 = M_PI / 25;
    float d2 = M_PI / 25;

    float mat_rotate[4][4]{ 0 };
    float mat_translate[4][4]{ 0 };
    float mat_scale[4][4]{ 0 };
};

class sphere : public object
{
public:
    sphere(int radius);
};

class game
{
public:
    virtual void   add_object(const object& obj) = 0;
    virtual void   remove_object(size_t index)   = 0;
    virtual object get_object(size_t index)      = 0;

protected:
    std::vector<object> objects;
};

class sphere_game : public game
{
public:
    sphere_game(engine* e);

    void   add_object(const object& obj) override;
    void   remove_object(size_t index) override;
    object get_object(size_t index) override;
};
