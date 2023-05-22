#include "chessboard_cells.h"

chessboard_cells::chessboard_cells()
{
    set_rotate(0, 0, 0);
    set_scale(1, 1, 1);
    set_translate(0, 0.075, 0);

    vertexes.push_back(vertex_textured(vertex(-0.5, 0, -0.5), 0, 0));
    vertexes.push_back(vertex_textured(vertex(-0.5, 0, 0.5), 0, 4));
    vertexes.push_back(vertex_textured(vertex(0.5, 0, -0.5), 4, 0));
    vertexes.push_back(vertex_textured(vertex(0.5, 0, 0.5), 4, 4));

    indexes.push_back(0);
    indexes.push_back(1);
    indexes.push_back(2);

    indexes.push_back(1);
    indexes.push_back(3);
    indexes.push_back(2);
    count = 2;

    for (size_t i = 0; i < get_count(); i++)
    {
        triangle tr = get_triangle(i);
        vertexes[indexes[3 * i + 0]].normal += tr.normal;
        vertexes[indexes[3 * i + 1]].normal += tr.normal;
        vertexes[indexes[3 * i + 2]].normal += tr.normal;
    }
}
