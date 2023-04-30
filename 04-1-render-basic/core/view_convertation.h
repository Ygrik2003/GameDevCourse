#pragma once

#include "matrix.h"

#include <iostream>

class view_convertation
{
public:
    view_convertation(double aspect,
                      double distance_front,
                      double distance_back)
    {
        this->aspect         = aspect;
        this->distance_front = distance_front;
        this->distance_back  = distance_back;

        projection_matrix.set_element(0, 0, 1 / (aspect * std::tan(fovy / 2)));
        projection_matrix.set_element(1, 1, 1 / std::tan(fovy / 2));

        projection_matrix.set_element(2,
                                      2,
                                      (distance_back + distance_front) /
                                          (distance_back - distance_front));
        projection_matrix.set_element(2,
                                      3,
                                      -2 * distance_back * distance_front /
                                          (distance_back - distance_front));
        projection_matrix.set_element(3, 2, 1);
        std::cout << projection_matrix.getElement(2, 2) << " "
                  << projection_matrix.getElement(2, 3) << std::endl;
    }

    void set_fovy(double fovy)
    {
        this->fovy = fovy;

        projection_matrix.set_element(0, 0, 1. / (aspect * std::tan(fovy / 2)));
        projection_matrix.set_element(1, 1, 1. / std::tan(fovy / 2));
    }
    void set_aspect(double aspect)
    {
        this->aspect = aspect;

        projection_matrix.set_element(0, 0, 1. / (aspect * std::tan(fovy / 2)));
    }
    void set_rotation()
    {
        rotation_matrix.set_element(0, 0, 1);
        rotation_matrix.set_element(1, 1, 1);
        rotation_matrix.set_element(2, 2, 1);
        rotation_matrix.set_element(3, 3, 1);
    }
    void set_translation(int x, int y, int z)
    {
        translation_matrix.set_element(3, 0, x);
        translation_matrix.set_element(3, 1, y);
        translation_matrix.set_element(3, 2, z);

        translation_matrix.set_element(0, 0, 1);
        translation_matrix.set_element(1, 1, 1);
        translation_matrix.set_element(2, 2, 1);
        translation_matrix.set_element(3, 3, 1);
    }

    void set_scale(double scale_x, double scale_y, double scale_z) {}

    matrix convert(int x, int y, int z)
    {
        matrix vec4 = matrix(1, 4);
        vec4.set_element(0, 0, x);
        vec4.set_element(0, 1, y);
        vec4.set_element(0, 2, z);
        vec4.set_element(0, 3, 1);

        matrix res = vec4 * translation_matrix * projection_matrix;
        if (res.getElement(0, 3) == 0)
            return res;
        return res / res.getElement(0, 3);
    }

private:
    double fovy           = M_PI / 3;
    double aspect         = 0;
    double distance_front = 0;
    double distance_back  = 0;

    matrix projection_matrix  = matrix(4, 4);
    matrix translation_matrix = matrix(4, 4);
    matrix scale_matrix       = matrix(4, 4);
    matrix rotation_matrix    = matrix(4, 4);
};