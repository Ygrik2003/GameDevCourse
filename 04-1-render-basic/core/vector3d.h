#pragma once
#include <cmath>
class vector3d
{
public:
    vector3d();
    vector3d(double x, double y, double z);

    double   dotMul(vector3d vector);
    vector3d crossMul(vector3d vector);

    void     normalize();
    vector3d getNormolized();
    double   getNorm();
    double   getNormSqr();

    vector3d operator+(vector3d vector);
    vector3d operator-(vector3d vector);
    vector3d operator/(double number);
    vector3d operator*(double number);

    double x, y, z;
};
