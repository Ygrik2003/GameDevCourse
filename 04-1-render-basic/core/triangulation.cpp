// TriangulationMy.cpp : Определяет точку входа для приложения.
//
#include "algorithm"
#include "camera.h"

using namespace std;

#define MAX_LOADSTRING 100
#define PI acos(-1)
#define IDT_TIMER 100

const double R = 1, dPhi = PI / 10, dTheta = PI / 10;

const double xStart = -2, xEnd = 2, dx = 0.2;
const double yStart = -1, yEnd = 1, dy = 0.2;
const double zStart = -1, zEnd = 1, dz = 0.2;

double phiCamera = 0, thetaCamera = PI / 3;

int       MaxX, MaxY, offsetX, offsetY, scaleX, scaleY;
RECT      rectWindow;
triangle* triangles;
int       index = 0;

camera camera;

HBRUSH brushWhite = CreateSolidBrush(RGB(255, 255, 255));

HPEN penRed   = CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
HPEN penGreen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
HPEN penBlue  = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));

const double FPS = 30;

POINT* vectorToPoint(Triangle triangle);

void     generate(HDC);
vector3d getSphere(double, double);
void     paintSphere();

vector3d getPyramid(double phi, double theta);
vector3d getPyramidPositive(double y, double z);
vector3d getPyramidNegative(double y, double z);
void     paintPyramid();

int x2ix(double);
int y2iy(double);

void sortZ(Triangle*, int);

int x2ix(double x)
{
    return offsetX + scaleX * x;
}

int y2iy(double y)
{
    return offsetY - scaleY * y;
}

POINT* vectorToPoint(Triangle triangle)
{
    POINT points[] = {
        { x2ix(triangle.vertexs[0].x), y2iy(triangle.vertexs[0].y) },
        { x2ix(triangle.vertexs[1].x), y2iy(triangle.vertexs[1].y) },
        { x2ix(triangle.vertexs[2].x), y2iy(triangle.vertexs[2].y) }
    };

    return points;
}

vector3d getSphere(double phi, double theta)
{
    return vector3d(
        R * cos(phi) * sin(theta), R * sin(phi) * sin(theta), R * cos(theta));
}

vector3d getPyramid(double phi, double theta)
{
    return vector3d(abs(R * cos(theta) + R * sin(phi) * sin(theta)) +
                        abs(R * cos(theta) - R * sin(phi) * sin(theta)),
                    R * sin(phi) * sin(theta),
                    R * cos(theta));
}

vector3d getPyramidPositive(double y, double z)
{
    return vector3d(abs(z + y) + abs(z - y), y, z);
}

vector3d getPyramidNegative(double y, double z)
{
    return vector3d(-abs(z + y) - abs(z - y), y, z);
}

void paintPyramid()
{
    if (triangles == NULL)
        triangles = new Triangle[(int)(2 * ((yEnd - yStart) / dy + 1) *
                                       ((zEnd - zStart) / dz + 1) * 2)];

    index = 0;

    double y = yStart, z = zStart;

    for (int yCount = 0; yCount < (yEnd - yStart) / dy; yCount++)
    {
        z = zStart;
        for (int zCount = 0; zCount < (zEnd - zStart) / dz; zCount++)
        {
            if ((z + dz) * (y + dy) < 0)
            {
                triangles[index].vertexs[0] = getPyramidPositive(y, z);
                triangles[index].vertexs[1] = getPyramidPositive(y, z + dz);
                triangles[index].vertexs[2] = getPyramidPositive(y + dy, z);
                triangles[index].calcTriangleNorm();
                triangles[index].brush = CreateSolidBrush(
                    RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255,
                        0,
                        0));

                index++;

                triangles[index].vertexs[0] = getPyramidPositive(y, z + dz);
                triangles[index].vertexs[1] =
                    getPyramidPositive(y + dy, z + dz);
                triangles[index].vertexs[2] = getPyramidPositive(y + dy, z);
                triangles[index].calcTriangleNorm();
                triangles[index].brush = CreateSolidBrush(
                    RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255,
                        0,
                        0));

                index++;
            }
            else
            {
                triangles[index].vertexs[0] = getPyramidPositive(y, z);
                triangles[index].vertexs[1] = getPyramidPositive(y, z + dz);
                triangles[index].vertexs[2] =
                    getPyramidPositive(y + dy, z + dz);
                triangles[index].calcTriangleNorm();
                triangles[index].brush = CreateSolidBrush(
                    RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255,
                        0,
                        0));

                index++;

                triangles[index].vertexs[0] = getPyramidPositive(y, z);
                triangles[index].vertexs[1] = getPyramidPositive(y + dy, z);
                triangles[index].vertexs[2] =
                    getPyramidPositive(y + dy, z + dz);
                triangles[index].calcTriangleNorm();
                triangles[index].brush = CreateSolidBrush(
                    RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255,
                        0,
                        0));

                index++;
            }
            z += dz;
        }
        y += dy;
    }

    y = yStart;
    for (int yCount = 0; yCount < (yEnd - yStart) / dy; yCount++)
    {
        z = zStart;
        for (int zCount = 0; zCount < (zEnd - zStart) / dz; zCount++)
        {
            if ((z + dz) * (y + dy) < 0)
            {
                triangles[index].vertexs[0] = getPyramidNegative(y, z);
                triangles[index].vertexs[1] = getPyramidNegative(y, z + dz);
                triangles[index].vertexs[2] = getPyramidNegative(y + dy, z);
                triangles[index].calcTriangleNorm();
                // brush
                index++;

                triangles[index].vertexs[0] = getPyramidNegative(y, z + dz);
                triangles[index].vertexs[1] =
                    getPyramidNegative(y + dy, z + dz);
                triangles[index].vertexs[2] = getPyramidNegative(y + dy, z);
                triangles[index].calcTriangleNorm();
                // brush
                index++;
            }
            else
            {
                triangles[index].vertexs[0] = getPyramidNegative(y, z);
                triangles[index].vertexs[1] = getPyramidNegative(y, z + dz);
                triangles[index].vertexs[2] =
                    getPyramidNegative(y + dy, z + dz);
                triangles[index].calcTriangleNorm();
                // brush
                index++;

                triangles[index].vertexs[0] = getPyramidNegative(y, z);
                triangles[index].vertexs[1] = getPyramidNegative(y + dy, z);
                triangles[index].vertexs[2] =
                    getPyramidNegative(y + dy, z + dz);
                triangles[index].calcTriangleNorm();
                // brush
                index++;
            }
            z += dz;
        }
        y += dy;
    }
}

vector3d getSpherePositive(double y, double z)
{
    return vector3d(sqrt(pow(R, 2) - pow(y, 2) - pow(z, 2)), y, z);
}

vector3d getSphereNegative(double y, double z)
{
    return vector3d(-sqrt(pow(R, 2) - pow(y, 2) - pow(z, 2)), y, z);
}

void paintSphere()
{

    index = 0;

    double theta = 0, phi;

    for (int thetaCounter = 0; thetaCounter <= PI / dTheta; thetaCounter++)
    {
        phi = 0;
        theta += dTheta;
        for (int phiCounter = 0; phiCounter <= 2 * PI / dPhi; phiCounter++)
        {
            phi += dPhi;
            triangles[index].vertexs[0] = getSphere(phi, theta);
            triangles[index].vertexs[1] = getSphere(phi, theta + dTheta);
            triangles[index].vertexs[2] = getSphere(phi + dPhi, theta);
            triangles[index].calcTriangleNorm();
            // brush
            index++;

            triangles[index].vertexs[0] = getSphere(phi, theta + dTheta);
            triangles[index].vertexs[1] = getSphere(phi + dPhi, theta + dTheta);
            triangles[index].vertexs[2] = getSphere(phi + dPhi, theta);

            // brush
            index++;
        }
    }
}
