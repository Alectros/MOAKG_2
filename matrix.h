#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cassert>

class MatrixDbl
{
public:
    MatrixDbl(const int cols, const int rows)
    {
        m_cols = cols;
        m_rows = rows;
        m_value = static_cast<double *>(std::malloc(sizeof (double) * cols * rows));
    }
    MatrixDbl(const MatrixDbl &another)
    {
        m_cols = another.m_cols;
        m_rows = another.m_rows;
        m_value = static_cast<double *>(std::malloc(sizeof (double) * m_cols * m_rows));
        for(int i = 0; i < m_cols * m_rows; i++)
            m_value[i] = another[i];
    }

    ~MatrixDbl()
    {
        std::free(m_value);
    }
    double operator()(const int row, const int col) const
    {
        assert(row >= 0 && row < m_rows && col >=0 && col < m_cols);
        return m_value[row * m_cols + col];
    }

    double& operator()(const int row, const int col)
    {
        assert(row >= 0 && row < m_rows && col >=0 && col < m_cols);
        return m_value[row * m_cols + col];
    }
    double operator[](const int ind) const
    {
        return m_value[ind];
    }

    double& operator[](const int ind)
    {
        return m_value[ind];
    }

    MatrixDbl operator+(double number) const
    {
        MatrixDbl result(*this);
        for(int ind = 0; ind < m_rows * m_cols; ind++)
            result[ind] += number;
        return result;
    }

    int cols() const;
    int rows() const;
private:
    double *m_value = nullptr;
    int m_cols = 0;
    int m_rows = 0;
};

#endif // MATRIX_H
