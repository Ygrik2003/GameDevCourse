#include "canvas_basic.h"

#include <iostream>
#include <stdlib.h>

int main()
{
    int    width  = 1920;
    int    height = 1080;
    canvas image1(width, height);
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            image1.set_pixel(i,
                             j,
                             { static_cast<uint8_t>(255 * i / width),
                               static_cast<uint8_t>(255 * j / height),
                               10 });
    image1.save_image("test.ppm");

    canvas image2(0, 0);
    image2.load_image("test.ppm");

    return EXIT_SUCCESS;
}