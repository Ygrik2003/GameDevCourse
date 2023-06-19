#define _USE_MATH_DEFINES
#include <cmath>

struct transformation_object
{
    float rotate[3][3]{ 0 };
    float scale[3]{ 0 };
    float translate[3]{ 0 };
};
struct transformation_camera
{
    float rotate[4][4]{ 0 };
    float scale[4][4]{ 0 };
    float translate[4][4]{ 0 };
    float projection[4][4]{ 0 };
};

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
