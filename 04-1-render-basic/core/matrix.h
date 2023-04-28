#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <vector>

struct msize
{
    msize(int h = 1, int w = 1)
    {
        height = h;
        width  = w;
    }
    int  height;
    int  width;
    bool operator!=(msize a)
    {
        return (height != a.height) && (width != a.width);
    }
};

class matrix_exception
{
public:
    enum errors
    {
        SizeError, // Несоответствие размеров
        IndexError, // Обращение к несуществующему элементу
        SumError, // Cложение матриц не соответствующих размеров
        SubError, // Разность матриц не соответствующих размеров
        MulError, // Умножение матриц не соответствующих размеров
        ZeroDivisionError, // Умножение матриц не соответствующих размеров
        DeterminantError, // Матрица не является квадратной
        ReversematrixError, // Определитель равен нулю
    };

    matrix_exception(errors type, msize a = { 0, 0 }, msize b = { 0, 0 });
    void printError();

private:
    std::string err, info;
};

class matrix
{
public:
    matrix();
    matrix(int height, int width);
    matrix(msize);
    matrix(const matrix&);

    void  setSize(int, int);
    void  setSize(msize);
    msize getSize() const;

    matrix operator+(matrix);
    matrix operator-(matrix);
    matrix operator*(matrix);
    matrix        operator*(const double);
    friend matrix operator*(const double, matrix);
    matrix        operator/(const double);
    matrix&       operator=(const matrix);

    std::vector<double> getRow(int i);    // Получить i строку
    std::vector<double> getColumn(int j); // Получить j столбец
    double getElement(int i, int j); // Получить i j элемент матрицы
    void set_element(
        int i,
        int j,
        double value); // Установить значение value для i j элемента матрицы

    double determinant(); // Определитель матрицы
    double algAddition(
        int i, int j); // Алгебраическое дополнение по i строки и j столбцу

    void transpose();  // Транспонированная матрица
    void reverse();    // Обратная матрица

    void delRow(int i);    // Удалить i строку
    void delColumn(int j); // Удалить j столбец

    void addRow(int i,
                std::vector<double>
                    row); // Добавить строку row после i строки исходной матрицы
    void addRow();
    void addColumn(int j,
                   std::vector<double> column); // Добавить столбец column после
                                                // j столбца исходной матрицы
    void addColumn();

    void swapRows(int i1, int i2); // Меняет местами i1 и i2 строки
    void swapColumns(int j1, int j2); // Меняет местами j1 и j2 столбцы
private:
    double                           det(std::vector<std::vector<double>>);
    std::vector<std::vector<double>> mat;
};
