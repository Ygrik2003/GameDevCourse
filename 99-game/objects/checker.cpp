#include "checker.h"

checker::checker()
{
    set_translate(0, 0.25, 0);
    set_rotate(0, 0, 0);
    set_scale(1, 1, 1);
    int num = 20;

    vertexes.push_back(vertex_textured(vertex(0, 0.025, 0), 0, 0));

    for (double phi = 0; phi <= 2 * M_PI; phi += M_PI / num)
    {
        vertexes.push_back(vertex_textured(
            vertex(0.06 * cos(phi), 0.025, 0.06 * sin(phi)), 1, 1));
    }

    for (double phi = 0; phi <= 2 * M_PI; phi += M_PI / num)
    {
        vertexes.push_back(vertex_textured(
            vertex(0.06 * cos(phi), -0.025, 0.06 * sin(phi)), 0, 0));
    }

    vertexes.push_back(vertex_textured(vertex(0, -0.025, 0), 1, 1));

    for (int i = 0; i < 2 * num + 1; i++)
    {
        indexes.push_back(0);
        indexes.push_back(i);
        indexes.push_back(i + 1);
    }

    for (int i = 1; i < 2 * num + 1; i++)
    {
        indexes.push_back(i);
        indexes.push_back(2 * num + 1 + i + 1);
        indexes.push_back(i + 1);

        indexes.push_back(i);
        indexes.push_back(2 * num + 1 + i);
        indexes.push_back(2 * num + 1 + i + 1);
    }

    for (int i = 0; i < 2 * num + 1; i++)
    {
        indexes.push_back((2 * num + 1) + i);
        indexes.push_back((2 * num + 1) + i + 1);
        indexes.push_back(2 * (2 * num + 1));
    }

    count = indexes.size() / 3;
}