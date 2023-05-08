#include "engine/engine.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <vector>

class camera
{
public:
    camera(float front, float back, float fovy, float aspect);
    void set_rotate(float alpha, float beta, float gamma);
    void set_scale(float x, float y, float z);

    void                   move(float dx, float dy, float dz);
    transformation_camera& get_transform();

private:
    void set_translate(float dx, float dy, float dz);

    transformation_camera tr_cam;
    struct
    {
        float x{}, y{}, z{};
    } pos;
};

class object
{
public:
    virtual void set_rotate(float alpha, float beta, float gamma);
    virtual void set_translate(float dx, float dy, float dz);
    virtual void set_scale(float x, float y, float z);
    virtual void set_discretization(float, float);

    virtual void calculate(){};

    virtual void render(engine* _engine, camera& _camera);

    uint     size() const;
    triangle get_triangle(const uint index);

    transformation_object get_transform() const { return tr_obj; }

protected:
    vertexes          _vertexes;
    std::vector<uint> indexes;
    uint              count_triangles = 0;

    // Step of discrete parameters
    float d1 = M_PI / 25;
    float d2 = M_PI / 25;

    transformation_object tr_obj;
};

using objects = std::vector<object>;

class sphere : public object
{
public:
    sphere(float radius);

private:
    uint   get_index(int phi_c, int theta_c);
    vertex get_sphere(double phi, double theta, uint r);
    void   calculate() override;

    float radius;
};

class game
{
public:
    virtual void    add_object(const object& obj) = 0;
    virtual void    remove_object(size_t index)   = 0;
    virtual object& get_object(size_t index)      = 0;

    auto begin() { return _objects.begin(); }
    auto end() { return _objects.end(); }

protected:
    objects _objects;
    engine* _engine;
};

class sphere_game : public game
{
public:
    sphere_game(engine* e);

    void    add_object(const object& obj) override;
    void    remove_object(size_t index) override;
    object& get_object(size_t index) override;
};
