#include "vector3d.h"

vector3d::vector3d()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

vector3d::vector3d(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

double vector3d::dotMul(vector3d vector)
{

    return this->x * vector.x + this->y * vector.y + this->z * vector.z;
}

vector3d vector3d::crossMul(vector3d vector)
{
    return vector3d(this->y * vector.z - this->z * vector.y,
                    this->z * vector.x - this->x * vector.z,
                    this->x * vector.y - this->y * vector.x);
}

void vector3d::normalize()
{
    double norm = getNorm();

    this->x /= norm;
    this->y /= norm;
    this->z /= norm;
}

vector3d vector3d::getNormolized()
{
    double norm = getNorm();
    return vector3d(this->x / norm, this->y / norm, this->z / norm);
}

double vector3d::getNorm()
{
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

double vector3d::getNormSqr()
{
    return pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2);
}

vector3d vector3d::operator+(vector3d vector)
{
    return vector3d(this->x + vector.x, this->y + vector.y, this->z + vector.z);
}

vector3d vector3d::operator-(vector3d vector)
{
    return vector3d(this->x - vector.x, this->y - vector.y, this->z - vector.z);
}

vector3d vector3d::operator/(double number)
{
    return vector3d(this->x / number, this->y / number, this->z / number);
}

vector3d vector3d::operator*(double number)
{
    return vector3d(this->x * number, this->y * number, this->z * number);
}
