#include "chessboard.h"

chessboard::chessboard()
{
    set_translate(0, 0.05, 0);
    set_scale(1, 1, 1);
    set_rotate(0, 0, 0);

    // UP INSIDE BORDER
    vertexes.push_back(vertex_textured(vertex(-0.5, 0.025, 0.5), 0.125, 0.875));
    vertexes.push_back(vertex_textured(vertex(0.5, 0.025, 0.5), 0.875, 0.875));
    vertexes.push_back(vertex_textured(vertex(0.5, 0.025, -0.5), 0.875, 0.125));
    vertexes.push_back(
        vertex_textured(vertex(-0.5, 0.025, -0.5), 0.125, 0.125));

    // UP BORDER
    vertexes.push_back(vertex_textured(vertex(-0.6, 0.05, 0.6), 0.1, 0.9));
    vertexes.push_back(vertex_textured(vertex(0.6, 0.05, 0.6), 0.9, 0.9));
    vertexes.push_back(vertex_textured(vertex(0.6, 0.05, -0.6), 0.9, 0.1));
    vertexes.push_back(vertex_textured(vertex(-0.6, 0.05, -0.6), 0.1, 0.1));

    // DOWN BORDER
    vertexes.push_back(vertex_textured(vertex(-0.6, -0.05, 0.6), 0, 1));
    vertexes.push_back(vertex_textured(vertex(0.6, -0.05, 0.6), 1, 1));
    vertexes.push_back(vertex_textured(vertex(0.6, -0.05, -0.6), 1, 0));
    vertexes.push_back(vertex_textured(vertex(-0.6, -0.05, -0.6), 0, 0));

    indexes.push_back(0);
    indexes.push_back(3);
    indexes.push_back(4);
    indexes.push_back(4);
    indexes.push_back(3);
    indexes.push_back(7);

    indexes.push_back(6);
    indexes.push_back(7);
    indexes.push_back(2);
    indexes.push_back(2);
    indexes.push_back(7);
    indexes.push_back(3);

    indexes.push_back(1);
    indexes.push_back(6);
    indexes.push_back(2);
    indexes.push_back(5);
    indexes.push_back(6);
    indexes.push_back(1);

    indexes.push_back(0);
    indexes.push_back(5);
    indexes.push_back(1);
    indexes.push_back(4);
    indexes.push_back(5);
    indexes.push_back(0);

    indexes.push_back(3 + 4);
    indexes.push_back(0 + 4);
    indexes.push_back(4 + 4);
    indexes.push_back(3 + 4);
    indexes.push_back(4 + 4);
    indexes.push_back(7 + 4);

    indexes.push_back(7 + 4);
    indexes.push_back(6 + 4);
    indexes.push_back(2 + 4);
    indexes.push_back(7 + 4);
    indexes.push_back(2 + 4);
    indexes.push_back(3 + 4);

    indexes.push_back(6 + 4);
    indexes.push_back(1 + 4);
    indexes.push_back(2 + 4);
    indexes.push_back(6 + 4);
    indexes.push_back(5 + 4);
    indexes.push_back(1 + 4);

    indexes.push_back(5 + 4);
    indexes.push_back(0 + 4);
    indexes.push_back(1 + 4);
    indexes.push_back(5 + 4);
    indexes.push_back(4 + 4);
    indexes.push_back(0 + 4);

    indexes.push_back(8);
    indexes.push_back(10);
    indexes.push_back(9);
    indexes.push_back(8);
    indexes.push_back(11);
    indexes.push_back(10);

    count = indexes.size() / 3;
}