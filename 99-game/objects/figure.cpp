#include "figure.h"

figure::figure()
{
    set_scale(1, 1, 1);
    set_rotate(0, 0, 0);
    set_translate(0, 0, 0);
}

void figure::add_figure(const figure& fig)
{
    for (auto ind : fig.indexes)
        this->indexes.push_back(vertexes.size() + ind);
    for (auto vert : fig.vertexes)
        this->vertexes.push_back(vert);
    count = indexes.size() / 3;
}