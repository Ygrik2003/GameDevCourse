#include "irender.h"

irender::~irender() {}

double position::length()
{
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

position operator-(const position& left, const position& right)
{
    return { left.x - right.x, left.y - right.y };
}

bool operator==(const position& left, const position& right)
{
    return left.x == right.x && left.y == right.y;
}

position position::generate(const int width, const int height)
{
    return position{ rand() % width, rand() % height };
}
