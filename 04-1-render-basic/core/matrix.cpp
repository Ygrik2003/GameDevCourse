#include "matrix.h"

matrix::matrix()
{
    this->set_size(1, 1);
}

matrix::matrix(int height, int width)
{
    this->set_size(height, width);
}

matrix::matrix(msize size)
{
    this->set_size(size);
}

matrix::matrix(const matrix& a)
{
    msize size = a.get_size();
    mat.resize(size.height);
    for (int i = 0; i < size.height; i++)
    {
        mat[i].resize(size.width);
        mat[i] = a.mat[i];
    }
}

void matrix::set_size(int height, int width)
{
    if (height <= 0 || width <= 0)
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(height, width));

    mat.resize(height);
    for (int i = 0; i < height; i++)
        mat[i].resize(width);
}

void matrix::set_size(msize size)
{
    if (size.height <= 0 || size.width <= 0)
        throw matrix_exception(matrix_exception::errors::IndexError, size);

    this->set_size(size.height, size.width);
}

msize matrix::get_size() const
{
    return msize(mat.size(), mat[0].size());
}

matrix matrix::operator+(matrix mat)
{
    if (this->get_size() != mat.get_size())
        throw matrix_exception(matrix_exception::errors::SumError,
                               this->get_size(),
                               mat.get_size());

    matrix temp(this->get_size());
    for (int i = 0; i < this->mat.size(); i++)
        for (int j = 0; j < this->mat[i].size(); j++)
            temp.mat[i][j] = this->mat[i][j] + mat.mat[i][j];
    return temp;
}

matrix matrix::operator-(matrix mat)
{
    if (this->get_size() != mat.get_size())
        throw matrix_exception(matrix_exception::errors::SubError,
                               this->get_size(),
                               mat.get_size());

    matrix temp(this->get_size());
    for (int i = 0; i < this->mat.size(); i++)
        for (int j = 0; j < this->mat[i].size(); j++)
            temp.mat[i][j] = this->mat[i][j] - mat.mat[i][j];
    return temp;
}

matrix matrix::operator*(const double number)
{
    matrix temp(this->get_size());
    for (int i = 0; i < this->mat.size(); i++)
        for (int j = 0; j < this->mat[i].size(); j++)
            temp.mat[i][j] = this->mat[i][j] * number;
    return temp;
}

matrix operator*(const double number, matrix mat)
{
    matrix temp(mat.get_size());
    for (int i = 0; i < mat.get_size().height; i++)
        for (int j = 0; j < mat.mat[i].size(); j++)
            temp.mat[i][j] = mat.mat[i][j] * number;
    return temp;
}

matrix matrix::operator*(matrix mat)
{
    if (this->mat[0].size() != mat.mat.size())
        throw matrix_exception(matrix_exception::errors::MulError,
                               this->get_size(),
                               mat.get_size());

    matrix temp(this->mat.size(), mat.mat[0].size());
    for (int i = 0; i < this->mat.size(); i++)
        for (int j = 0; j < this->mat[i].size(); j++)
            for (int k = 0; k < mat.mat.size(); k++)
                temp.mat[i][j] += this->mat[i][k] * mat.mat[k][j];
    return temp;
}

matrix matrix::operator/(const double number)
{
    if (number == 0)
        throw matrix_exception(matrix_exception::errors::ZeroDivisionError);

    matrix temp(this->get_size());
    for (int i = 0; i < this->mat.size(); i++)
        for (int j = 0; j < this->mat[i].size(); j++)
            temp.mat[i][j] = this->mat[i][j] / number;
    return temp;
}

matrix& matrix::operator=(const matrix mat)
{
    this->mat.resize(mat.mat.size());
    for (int i = 0; i < this->mat.size(); i++)
    {
        this->mat[i] = mat.mat[i];
    }
    return *this;
}

double matrix::determinant()
{
    if (this->mat[0].size() != mat.size())
        throw matrix_exception(matrix_exception::errors::DeterminantError,
                               get_size());

    return det(mat);
}

double matrix::alg_addition(int i, int j)
{
    matrix temp;
    double a = mat[i][j];
    temp.mat = mat;
    temp.del_row(i);
    temp.del_column(j);
    return a * pow(-1, i + j) * temp.determinant();
}

void matrix::transpose()
{
    matrix temp(mat[0].size(), mat.size());
    for (int i = 0; i < mat.size(); i++)
        for (int j = 0; j < mat[i].size(); j++)
            temp.mat[j][i] = mat[i][j];
    mat = temp.mat;
}

void matrix::reverse()
{
    double d = determinant();
    if (d == 0)
        throw matrix_exception(matrix_exception::errors::ReversematrixError);

    matrix tmp(mat.size(), mat.size());
    for (int i = 0; i < mat.size(); i++)
        for (int j = 0; j < mat.size(); j++)
            tmp.mat[i][j] = (1 / d) * alg_addition(i, j);
    mat = tmp.mat;
}

std::vector<double> matrix::get_row(int i)
{
    if (i >= mat.size() || i < 0)
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(i, 0));
    return mat[i];
}

std::vector<double> matrix::get_column(int j)
{
    if (j >= mat[0].size() || j < 0)
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(0, j));
    std::vector<double> tmp;
    for (int i = 0; i < this->get_size().height; i++)
        tmp.push_back(mat[i][j]);
    return tmp;
}

double matrix::get_element(int i, int j)
{
    if ((i >= mat.size() || i < 0) || (j >= mat[0].size() || j < 0))
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(i, j));
    return mat[i][j];
}

void matrix::set_element(int i, int j, double value)
{
    if ((i >= mat.size() || i < 0) || (j >= mat[0].size() || j < 0))
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(i, j));
    mat[i][j] = value;
}

void matrix::del_row(int i)
{
    if (i >= mat.size() || i < 0)
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(i, 0));
    mat.erase(mat.begin() + i);
}

void matrix::del_column(int j)
{
    if (j >= mat[0].size() || j < 0)
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(0, j));
    for (int i = 0; i < mat.size(); i++)
        mat[i].erase(mat[i].begin() + j);
}

void matrix::add_row(int i, std::vector<double> a)
{
    if (i >= mat.size() || i < 0)
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(i, 0));
    if (mat[0].size() != a.size())
        throw matrix_exception(matrix_exception::errors::SizeError,
                               get_size(),
                               msize(1, a.size()));
    mat.insert(mat.begin() + i, a);
}

void matrix::add_row()
{
    std::vector<double> tmp;
    tmp.resize(mat[0].size());
    mat.push_back(tmp);
}

void matrix::add_column(int j, std::vector<double> a)
{
    if (j >= mat[0].size() || j < 0)
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(0, j));
    if (mat.size() != a.size())
        throw matrix_exception(matrix_exception::errors::SizeError,
                               get_size(),
                               msize(a.size(), 1));
    for (int i = 0; i < a.size(); i++)
        mat[i].insert(mat[i].begin() + j, a[i]);
}

void matrix::add_column()
{
    for (int i = 0; i < mat.size(); i++)
        mat[i].push_back(0);
}

void matrix::swap_rows(int i1, int i2)
{
    if (i1 >= mat.size() || i1 < 0)
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(i1, 0));
    if (i2 >= mat.size() || i2 < 0)
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(i2, 0));
    std::swap(mat[i1], mat[i2]);
}

void matrix::swap_columns(int j1, int j2)
{
    if (j1 >= mat[0].size() || j1 < 0)
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(0, j1));
    if (j2 >= mat[0].size() || j2 < 0)
        throw matrix_exception(matrix_exception::errors::IndexError,
                               msize(0, j2));

    for (int i = 0; i < mat[0].size(); i++)
        std::swap(mat[i][j1], mat[i][j2]);
}

double matrix::det(std::vector<std::vector<double>> a)
{
    if (a.size() == 1)
        return a[0][0];
    double                           d = 0;
    std::vector<std::vector<double>> tmp;
    for (int j = 0; j < a.size(); j++)
    { // Разложение по первой строке (i = 0)
        tmp = a;
        tmp.erase(tmp.begin());
        for (int i = 0; i < tmp.size(); i++)
            tmp[i].erase(tmp[i].begin() + j);
        d += a[0][j] * pow(-1, j) * det(tmp);
    }
    return d;
}

matrix_exception::matrix_exception(errors type, msize a, msize b)
{
    char* tmp;
    switch (type)
    {
        case matrix_exception::SumError:
            err = "SumError";
            std::sprintf(
                tmp,
                "Can't summary matrix's sizes of (%d, %d) and (%d, %d)",
                a.height,
                a.width,
                b.height,
                b.width);
            info = tmp;
            break;
        case matrix_exception::SubError:
            err = "SubError";
            std::sprintf(
                tmp,
                "Can't subtraction matrix's sizes of (%d, %d) and (%d, %d)",
                a.height,
                a.width,
                b.height,
                b.width);
            info = tmp;
            break;
        case matrix_exception::MulError:
            err = "MulError";
            std::sprintf(
                tmp,
                "Can't multiplied matrix's sizes of (%d, %d) and (%d, %d)",
                a.height,
                a.width,
                b.height,
                b.width);
            info = tmp;
            break;
        case matrix_exception::ZeroDivisionError:
            err  = "ZeroDivisionError";
            info = "Can't division by zero";
            break;
        case matrix_exception::ReversematrixError:
            err  = "ReversematrixError";
            info = "Can't calculate reverse matrix with det() = 0";
            break;
        case matrix_exception::IndexError:
            err = "IndexError";
            std::sprintf(
                tmp, "Can't treat to element (%d, %d)", a.height, a.width);
            info = tmp;
            break;
        case matrix_exception::DeterminantError:
            err = "DeterminateError";
            std::sprintf(
                tmp,
                "Can't calculate determinate of matrix with size (%d, %d)",
                a.height,
                a.width);
            info = tmp;
            break;
        case matrix_exception::errors::SizeError:
            err = "SizeError";
            std::sprintf(tmp,
                         "Can't do this operation with matrix sizes of (%d, "
                         "%d) and (%d, %d)",
                         a.height,
                         a.width,
                         b.height,
                         b.width);
            info = tmp;
            break;
    }
    std::printf("[%1]: %2", err, info);
}

void matrix_exception::print_error()
{
    std::printf("[%1]: %2", err, info);
}
