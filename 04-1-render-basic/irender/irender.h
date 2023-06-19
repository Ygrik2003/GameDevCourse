#include "canvas_basic.h"

#include <cmath>
#include <cstdint>

struct position
{
    double          length();
    friend position operator-(const position& left, const position& right);
    friend bool     operator==(const position& left, const position& right);
    int32_t         x = 0;
    int32_t         y = 0;

    static position generate(const int width, const int height);
};

using pixels = std::vector<position>;

class irender
{
public:
    virtual void   clear(rgb)                                     = 0;
    virtual void   set_pixel(position, double, rgb)               = 0;
    virtual pixels pixels_positions(position start, position end) = 0;

    virtual ~irender();

protected:
    std::vector<double> z_buffer;
};