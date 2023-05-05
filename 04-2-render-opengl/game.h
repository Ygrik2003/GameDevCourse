#include "engine/engine.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <vector>

struct rgb
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct vertex
{
    double x;
    double y;
    double z;
    rgb    color;
};

using vertexes = std::vector<vertex>;

class object
{
public:
    virtual void set_rotate(double alpha, double beta, double gamma) = 0;
    virtual void set_translate(double dx, double dy, double dz)      = 0;
    virtual void set_scale(double x, double y, double z)             = 0;
    virtual void set_discretization(double, double)                  = 0;

    // Step of discrete parameters
    double d1 = M_PI / 25;
    double d2 = M_PI / 25;

    vertexes vertexes_object;
};

class sphere : public object
{
public:
    sphere(int radius);

    void set_rotate(double alpha, double beta, double gamma) override;
    void set_translate(double dx, double dy, double dz) override;
    void set_scale(double x, double y, double z) override;
    void set_discretization(double, double) override;
};

class game
{
public:
    virtual ~game()                              = 0;
    virtual void   add_object(const object& obj) = 0;
    virtual void   remove_object(size_t index)   = 0;
    virtual object get_object(size_t index)      = 0;

protected:
    std::vector<object> objects;
};

class sphere_game : public game
{

    sphere_game(engine& e);
    ~sphere_game() = default;

    void   add_object(const object& obj) override;
    void   remove_object(size_t index) override;
    object get_object(size_t index) override;
};
