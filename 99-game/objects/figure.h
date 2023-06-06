#pragma once
#include "object.h"

class figure : public object
{
public:
    figure();
    auto get_triangle(size_t index)
    {
        return triangle(vertexes[indexes[3 * index + 0]],
                        vertexes[indexes[3 * index + 1]],
                        vertexes[indexes[3 * index + 2]]);
    }
    size_t                                get_count() const { return count; }
    const std::vector<vertex3d_textured>& get_vertexes() const
    {
        return vertexes;
    }
    const std::vector<uint16_t>& get_indexes() const { return indexes; }

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

    void   set_texture_index(size_t index) { texture_index = index; }
    size_t get_texture_index() { return texture_index; }

    void add_figure(const figure& fig);

protected:
    std::vector<vertex3d_textured> vertexes;
    std::vector<uint16_t>          indexes;

    size_t count;

private:
    size_t texture_index = 0;
};