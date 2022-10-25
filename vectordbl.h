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

    VectorDbl operator*(const MatrixDbl &another) const
    {
        assert(m_rows == another.rows());
        VectorDbl result(m_rows, Type::DefaultValue, 0.0);
        for(int row = 0; row < m_rows; row++)
                for(int i = 0; i < m_cols; i++)
                    result(row) += (*this)(i) * another(i, row);
        return result;
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
        assert(std::abs(l) < std::numeric_limits<double>::epsilon());
        for(int i = 0; i < m_rows; i++)
            v[i] /= l;
        return v;
    }

    void normilize()
    {
        const double l = length();
        assert(std::abs(l) < std::numeric_limits<double>::epsilon());
        for(int i = 0; i < m_rows; i++)
            m_value[i] /= l;
    }

};

#endif // VECTORDBL_H