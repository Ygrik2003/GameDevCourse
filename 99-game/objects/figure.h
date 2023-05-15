#pragma once
#include "object.h"

class figure : public object
{
public:
    auto get_triangle(size_t index)
    {
        return triangle(vertexes[indexes[3 * index + 0]],
                        vertexes[indexes[3 * index + 1]],
                        vertexes[indexes[3 * index + 2]]);
    }
    size_t get_count() const { return count; }

    void uniform_link(uniform& uni) override
    {

        uni.rotate_alpha_obj = &alpha;
        uni.rotate_beta_obj  = &beta;
        uni.rotate_gamma_obj = &gamma;

        uni.translate_x_obj = &dx;
        uni.translate_y_obj = &dy;
        uni.translate_z_obj = &dz;

        uni.scale_x_obj = &scale_x;
        uni.scale_y_obj = &scale_y;
        uni.scale_z_obj = &scale_z;
    }

protected:
    std::vector<vertex_textured> vertexes;
    std::vector<uint32_t>        indexes;

    size_t count;
};