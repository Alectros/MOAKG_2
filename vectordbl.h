#ifndef VECTORDBL_H
#define VECTORDBL_H

#include "matrixdbl.h"
#include <cmath>

class VectorDbl : public MatrixDbl
{

public:
    VectorDbl(const int size, const Type type = Type::DefaultValue, const double defaultValue = 0)
        : MatrixDbl(size, 1, type, defaultValue) {}
    VectorDbl(const int size, const double *values) : MatrixDbl(size, 1, values){};
    VectorDbl(const MatrixDbl &mx) : MatrixDbl(mx.rows(), 1) {
        for(int i = 0; i < mx.rows(); i++)
            m_value[i] = mx(i, 0);
    };
    VectorDbl(const int size, const std::initializer_list<double> &values) : MatrixDbl(size, 1, values) {};

    double operator()(const int row, const int col) const = delete;
    double& operator()(const int row, const int col) = delete;

    double operator()(const int ind) const
    {
        assert(ind >= 0 && ind < m_rows);
        return (*this)[ind];
    }

    double& operator()(const int ind)
    {
        assert(ind >= 0 && ind < m_rows);
        return (*this)[ind];
    }

    VectorDbl transposed() const = delete;
    void transpose() = delete;

    double length() const
    {
        double sqrSum = 0;
        for(int i = 0; i < m_rows; i++)
            sqrSum += m_value[i] * m_value[i];
        return std::sqrt(sqrSum);
    }

    VectorDbl normilized() const
    {
        VectorDbl v(*this);
        const double l = length();
        assert(std::abs(l) > std::numeric_limits<double>::epsilon());
        for(int i = 0; i < m_rows; i++)
            v[i] /= l;
        return v;
    }

    void normilize()
    {
        const double l = length();
        assert(std::abs(l) > std::numeric_limits<double>::epsilon());
        for(int i = 0; i < m_rows; i++)
            m_value[i] /= l;
    }

};

inline VectorDbl operator*(const VectorDbl &v, const double &number)
{
    const MatrixDbl result = MatrixDbl(v) * number;
    return VectorDbl(result);
}

inline VectorDbl operator*(const double &number, const VectorDbl &v)
{
    const MatrixDbl result = MatrixDbl(v) * number;
    return VectorDbl(result);
}

inline VectorDbl operator-(const VectorDbl &v1, const VectorDbl &v2)
{
    const MatrixDbl result = MatrixDbl(v1) - MatrixDbl(v2);
    return VectorDbl(result);
}

inline VectorDbl operator+(const VectorDbl &v1, const VectorDbl &v2)
{
    const MatrixDbl result = MatrixDbl(v1) + MatrixDbl(v2);
    return VectorDbl(result);
}

inline VectorDbl operator*(const MatrixDbl &mx, const VectorDbl v)
{
    assert(v.rows() == mx.rows());
    VectorDbl result(v.rows(), MatrixDbl::Type::DefaultValue, 0.0);
    for(int row = 0; row < v.rows(); row++)
        for(int i = 0; i < mx.cols(); i++)
            result(row) += v(i) * mx(row, i);
    return result;
}

inline VectorDbl operator*(const VectorDbl v, const MatrixDbl &mx)
{
    assert(v.rows() == mx.rows());
    VectorDbl result(v.rows(), MatrixDbl::Type::DefaultValue, 0.0);
    for(int col = 0; col < v.rows(); col++)
        for(int i = 0; i < mx.rows(); i++)
            result(col) += v(i) * mx(i, col);
    return result;
}

#endif // VECTORDBL_H
