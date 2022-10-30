#ifndef MATRIXDBL3X3_H
#define MATRIXDBL3X3_H

#include <cmath>

#include "matrixdbl.h"
#include "vectordbl.h"

const double DEG_TO_RAD = M_PI / 180.0;

class MatrixDbl3x3 : public MatrixDbl
{
public:
    MatrixDbl3x3() : MatrixDbl(3, 3, Type::Unit) {}
    MatrixDbl3x3(double *values) : MatrixDbl(3, 3, values) {}
    MatrixDbl3x3(const MatrixDbl &mx) : MatrixDbl(3, 3, mx.value()) {}

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
    VectorDbl3(const double *values) : VectorDbl(3, values) {};
    VectorDbl3(const VectorDbl &v) : VectorDbl(3, v.value()) {};
    VectorDbl3(const double x, const double y, const double z) : VectorDbl(3)
    {
        m_value[0] = x;
        m_value[1] = y;
        m_value[2] = z;
    };
    double x() const
    {
        assert(std::abs(m_value[2]) > std::numeric_limits<double>::epsilon());
        return m_value[0] / m_value[2];
    }

    double y() const
    {
        assert(std::abs(m_value[2]) > std::numeric_limits<double>::epsilon());
        return m_value[1] / m_value[2];
    }

};

inline MatrixDbl3x3 operator*(const MatrixDbl3x3 &mx1, const MatrixDbl3x3 &mx2)
{
    const MatrixDbl result = MatrixDbl(mx1) * MatrixDbl(mx2);
    return MatrixDbl3x3(result);
}

inline VectorDbl3 operator*(const MatrixDbl3x3 &mx, const VectorDbl3 v)
{
    const VectorDbl result = MatrixDbl(mx) * VectorDbl(v);
    return VectorDbl3(result);
}

inline VectorDbl3 operator*(const VectorDbl3 v, const MatrixDbl3x3 &mx)
{
    const VectorDbl result = VectorDbl(v) * MatrixDbl(mx);
    return VectorDbl3(result);
}

inline VectorDbl3 operator-(const VectorDbl3 &v1, const VectorDbl3 v2)
{
    const VectorDbl result = VectorDbl(v1) - VectorDbl(v2);
    return VectorDbl3(result);
}

inline VectorDbl3 operator+(const VectorDbl3 &v1, const VectorDbl3 v2)
{
    const VectorDbl result = VectorDbl(v1) + VectorDbl(v2);
    return VectorDbl3(result);
}

#endif // MATRIXDBL3X3_H
