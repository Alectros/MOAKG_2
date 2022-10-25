#ifndef MATRIXDBL3X3_H
#define MATRIXDBL3X3_H

#include "matrixdbl.h"
#include "vectordbl.h"
#include <cmath>

const double DEG_TO_RAD = M_PI / 180.0;

class MatrixDbl3x3 : public MatrixDbl
{
public:
    MatrixDbl3x3() : MatrixDbl(3, 3, Type::Unit) {}

    static MatrixDbl3x3 unit()
    {
        return MatrixDbl3x3();
    }

    static MatrixDbl3x3 translation(const double x, const double y)
    {
        MatrixDbl3x3 mx;
        mx(0, 2) = x;
        mx(1, 2) = y;
        return mx;
    }

    static MatrixDbl3x3 rotation(const double angle)
    {
        MatrixDbl3x3 mx;
        mx(0, 0) = std::cos(angle * DEG_TO_RAD);
        mx(0, 1) = -std::sin(angle * DEG_TO_RAD);
        mx(1, 0) = std::sin(angle * DEG_TO_RAD);
        mx(1, 1) = std::cos(angle * DEG_TO_RAD);
        return mx;
    }

    static MatrixDbl3x3 scale(const double x, const double y)
    {
        MatrixDbl3x3 mx;
        mx(0, 0) = x;
        mx(1, 1) = y;
        return mx;
    }

    static MatrixDbl3x3 reflection(const bool isX, const bool isY)
    {
        MatrixDbl3x3 mx;
        mx(0, 0) *= isX ? -1 : 1;
        mx(1, 1) *= isY ? -1 : 1;
        return mx;
    }

};

class VectorDbl3 : public VectorDbl
{
public:
    VectorDbl3() : VectorDbl(3) {};
    VectorDbl3(const double *values) : VectorDbl(3, values){};
    VectorDbl3(const double x, const double y, const double z) : VectorDbl(3)
    {
        m_value[0] = x;
        m_value[1] = y;
        m_value[2] = z;
    };

};

MatrixDbl3x3 operator*(const MatrixDbl3x3 &first, const MatrixDbl3x3 &second)
{
    MatrixDbl result = first * second;
    return *dynamic_cast<MatrixDbl3x3*>(&result);
}

VectorDbl3 operator*(const MatrixDbl3x3 &mx, const VectorDbl3 v)
{
    VectorDbl result = mx * v;
    return *dynamic_cast<VectorDbl3*>(&result);
}

VectorDbl3 operator*(const VectorDbl3 v, const MatrixDbl3x3 &mx)
{
    VectorDbl result = v * mx;
    return *dynamic_cast<VectorDbl3*>(&result);
}

#endif // MATRIXDBL3X3_H
