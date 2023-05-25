#include "figure.h"

class mesh : public figure
{
public:
    mesh(std::vector<vertex_textured> vertexes, std::vector<uint32_t> indexes);
};