#ifndef AFFINETOOLS3D_H
#define AFFINETOOLS3D_H

#include <cmath>

#include "matrixdbl.h"
#include "vectordbl.h"


class VectorDbl4 : public VectorDbl
{
public:
    VectorDbl4() : VectorDbl(4) {};
    VectorDbl4(const double *values) : VectorDbl(4, values) {};
    VectorDbl4(const VectorDbl &v) : VectorDbl(4, v.value()) {};
    VectorDbl4(const double x, const double y, const double z, const double w) : VectorDbl(4)
    {
        m_value[0] = x;
        m_value[1] = y;
        m_value[2] = z;
        m_value[3] = w;
    };

    VectorDbl4(const std::initializer_list<double> &values) : VectorDbl(4, values) {}

    double x() const
    {
        assert(std::abs(m_value[3]) > std::numeric_limits<double>::epsilon());
        return m_value[0] / m_value[3];
    }

    double y() const
    {
        assert(std::abs(m_value[3]) > std::numeric_limits<double>::epsilon());
        return m_value[1] / m_value[3];
    }

    double z() const
    {
        assert(std::abs(m_value[3]) > std::numeric_limits<double>::epsilon());
        return m_value[2] / m_value[3];
    }

};

class MatrixDbl4x4 : public MatrixDbl
{
public:
    MatrixDbl4x4() : MatrixDbl(4, 4, Type::Unit) {}
    MatrixDbl4x4(double *values) : MatrixDbl(4, 4, values) {}
    MatrixDbl4x4(const MatrixDbl &mx) : MatrixDbl(4, 4, mx.value()) {}
    MatrixDbl4x4(const std::initializer_list<double> &values) :MatrixDbl(4, 4, values) {}

    static MatrixDbl4x4 unit()
    {
        return MatrixDbl4x4();
    }

    static MatrixDbl4x4 translation(const double x, const double y, const double z)
    {
        MatrixDbl4x4 mx;
        mx(0, 3) = x;
        mx(1, 3) = y;
        mx(2, 3) = z;
        return mx;
    }

    static MatrixDbl4x4 rotationZ(const double angle)
    {
        const double DEG_TO_RAD = M_PI / 180.0;
        MatrixDbl4x4 mx;
        mx(0, 0) = std::cos(angle * DEG_TO_RAD);
        mx(0, 1) = -std::sin(angle * DEG_TO_RAD);
        mx(1, 0) = std::sin(angle * DEG_TO_RAD);
        mx(1, 1) = std::cos(angle * DEG_TO_RAD);
        return mx;
    }

    static MatrixDbl4x4 rotationY(const double angle)
    {
        const double DEG_TO_RAD = M_PI / 180.0;
        MatrixDbl4x4 mx;
        mx(0, 0) = std::cos(angle * DEG_TO_RAD);
        mx(0, 2) = std::sin(angle * DEG_TO_RAD);
        mx(2, 0) = -std::sin(angle * DEG_TO_RAD);
        mx(2, 2) = std::cos(angle * DEG_TO_RAD);
        return mx;
    }

    static MatrixDbl4x4 rotationX(const double angle)
    {
        const double DEG_TO_RAD = M_PI / 180.0;
        MatrixDbl4x4 mx;
        mx(1, 1) = std::cos(angle * DEG_TO_RAD);
        mx(1, 2) = -std::sin(angle * DEG_TO_RAD);
        mx(2, 1) = std::sin(angle * DEG_TO_RAD);
        mx(2, 2) = std::cos(angle * DEG_TO_RAD);
        return mx;
    }

//    static MatrixDbl4x4 rotationAxe(const VectorDbl4 &normal, const double angle)
//    {
//        const VectorDbl4 n = normal.normilized();
//        const double DEG_TO_RAD = M_PI / 180.0;
//        const double cosA = std::cos(angle * DEG_TO_RAD);
//        const double sinA = std::sin(angle * DEG_TO_RAD);
//        MatrixDbl4x4 mx = {
//            cosA * cosA + (1 - cosA) * n.x() * n.x(),



//        }
//        return mx;
//    }

    static MatrixDbl4x4 scale(const double x, const double y, const double z)
    {
        MatrixDbl4x4 mx;
        mx(0, 0) = x;
        mx(1, 1) = y;
        mx(2, 2) = z;
        return mx;
    }

    static MatrixDbl4x4 reflection(const bool isX, const bool isY, const bool isZ)
    {
        MatrixDbl4x4 mx;
        mx(0, 0) *= isX ? -1 : 1;
        mx(1, 1) *= isY ? -1 : 1;
        mx(2, 2) *= isZ ? -1 : 1;
        return mx;
    }

};

inline MatrixDbl4x4 operator*(const MatrixDbl4x4 &mx1, const MatrixDbl4x4 &mx2)
{
    const MatrixDbl result = MatrixDbl(mx1) * MatrixDbl(mx2);
    return MatrixDbl4x4(result);
}

inline VectorDbl4 operator*(const MatrixDbl4x4 &mx, const VectorDbl4 v)
{
    const VectorDbl result = MatrixDbl(mx) * VectorDbl(v);
    return VectorDbl4(result);
}

inline VectorDbl4 operator*(const VectorDbl4 v, const MatrixDbl4x4 &mx)
{
    const VectorDbl result = VectorDbl(v) * MatrixDbl(mx);
    return VectorDbl4(result);
}

inline VectorDbl4 operator*(const VectorDbl4 v, const double &d)
{
    const VectorDbl result = VectorDbl(v) * d;
    return VectorDbl4(result);
}

inline VectorDbl4 operator*(const double &d, const VectorDbl4 v)
{
    const VectorDbl result = VectorDbl(v) * d;
    return VectorDbl4(result);
}

inline VectorDbl4 operator-(const VectorDbl4 &v1, const VectorDbl4 v2)
{
    const VectorDbl result = VectorDbl(v1) - VectorDbl(v2);
    return VectorDbl4(result);
}

inline VectorDbl4 operator+(const VectorDbl4 &v1, const VectorDbl4 v2)
{
    const VectorDbl result = VectorDbl(v1) + VectorDbl(v2);
    return VectorDbl4(result);
}

#endif // AFFINETOOLS3D_H
