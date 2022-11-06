#ifndef MATRIXDBL_H
#define MATRIXDBL_H

#include <cassert>
#include <initializer_list>
#include <limits>

class MatrixDbl
{
public:
    enum class Type
    {
        NotInitialized = 0,
        DefaultValue = 1,
        Unit = 2,
        UnitDefaultValue = 3
    };

    MatrixDbl(const int rows, const int cols, const Type type = Type::Unit, const double defaultValue = 0)
    {
        m_cols = cols;
        m_rows = rows;
        m_value = static_cast<double *>(std::malloc(sizeof (double) * cols * rows));

        if (type != Type::NotInitialized)
        for(int row = 0; row < m_rows; row++)
            for(int col = 0; col < m_cols; col++) {
                if (row == col) {
                    switch (type) {
                    case Type::Unit:
                        m_value[row * m_cols + col] = 1;
                        break;
                    case Type::DefaultValue:
                        m_value[row * m_cols + col] = defaultValue;
                        break;
                    case Type::UnitDefaultValue:
                        m_value[row * m_cols + col] = defaultValue;
                        break;
                    default:
                        break;
                    }
                } else {
                    switch (type) {
                    case Type::Unit:
                        m_value[row * m_cols + col] = 0.0;
                        break;
                    case Type::DefaultValue:
                        m_value[row * m_cols + col] = defaultValue;
                        break;
                    case Type::UnitDefaultValue:
                        m_value[row * m_cols + col] = 0.0;
                        break;
                    default:
                        break;
                    }
                }
            }
    }
    MatrixDbl(const int rows, const int cols, const double *values)
    {
        m_cols = cols;
        m_rows = rows;
        m_value = static_cast<double *>(std::malloc(sizeof (double) * m_cols * m_rows));
        for(int i = 0; i < m_cols * m_rows; i++)
            m_value[i] = values[i];
    }

    MatrixDbl(const int rows, const int cols, const std::initializer_list<double> &values)
    {
        m_cols = cols;
        m_rows = rows;
        m_value = static_cast<double *>(std::malloc(sizeof (double) * m_cols * m_rows));
        int index = 0;
        for(auto it = values.begin(); it != values.end() && index < m_cols * m_rows; ++it) {
            m_value[index] = *it;
            ++index;
        }
    }

    MatrixDbl(const MatrixDbl &another)
    {
        m_cols = another.m_cols;
        m_rows = another.m_rows;
        m_value = static_cast<double *>(std::malloc(sizeof (double) * m_cols * m_rows));
        for(int i = 0; i < m_cols * m_rows; i++)
            m_value[i] = another[i];
    }

    virtual ~MatrixDbl()
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
        assert(ind >= 0 && ind < m_cols * m_rows);
        return m_value[ind];
    }

    double& operator[](const int ind)
    {
        assert(ind >= 0 && ind < m_cols * m_rows);
        return m_value[ind];
    }

    void operator=(const MatrixDbl &another)
    {
        if (m_cols * m_rows != another.m_cols * another.m_rows) {
            std::free(m_value);
            m_value = static_cast<double *>(std::malloc(sizeof (double) *another.m_cols * another.m_rows));
        }
        m_cols = another.m_cols;
        m_rows = another.m_rows;
        for(int i = 0; i < m_cols * m_rows; i++)
            m_value[i] = another[i];
    }

    MatrixDbl operator+(const double &number) const
    {
        MatrixDbl result(*this);
        for(int ind = 0; ind < m_rows * m_cols; ind++)
            result[ind] += number;
        return result;
    }

    MatrixDbl operator-(const double &number) const
    {
        MatrixDbl result(*this);
        for(int ind = 0; ind < m_rows * m_cols; ind++)
            result[ind] -= number;
        return result;
    }

    MatrixDbl operator*(const double &number) const
    {
        MatrixDbl result(*this);
        for(int ind = 0; ind < m_rows * m_cols; ind++)
            result[ind] *= number;
        return result;
    }

    MatrixDbl operator/(const double &number) const
    {
        assert(std::abs(number) > std::numeric_limits<double>::epsilon());
        MatrixDbl result(*this);
        for(int ind = 0; ind < m_rows * m_cols; ind++)
            result[ind] /= number;
        return result;
    }

    MatrixDbl operator+(const MatrixDbl &another) const
    {
        assert(m_cols == another.m_cols && m_rows == another.m_rows);
        MatrixDbl result(m_rows, m_cols, Type::DefaultValue, 0.0);
        for(int row = 0; row < m_rows; row++)
            for(int col = 0; col < another.m_cols; col++)
                result(row, col) = (*this)(row, col) + another(row, col);
        return result;
    }

    MatrixDbl operator-(const MatrixDbl &another) const
    {
        assert(m_cols == another.m_cols && m_rows == another.m_rows);
        MatrixDbl result(m_rows, m_cols, Type::DefaultValue, 0.0);
        for(int row = 0; row < m_rows; row++)
            for(int col = 0; col < another.m_cols; col++)
                result(row, col) = (*this)(row, col) - another(row, col);
        return result;
    }

    MatrixDbl operator*(const MatrixDbl &another) const
    {
        assert(m_cols == another.m_rows);
        MatrixDbl result(m_rows, another.m_cols, Type::DefaultValue, 0.0);
        for(int row = 0; row < m_rows; row++)
            for(int col = 0; col < another.m_cols; col++) {
                for(int i = 0; i < m_cols; i++)
                    result(row, col) += (*this)(row, i) * another(i, col);
            }
        return result;
    }

    MatrixDbl transposed() const
    {
        MatrixDbl matrix(m_cols, m_rows, Type::NotInitialized);
        for(int i = 0; i < m_rows; i++)
            for(int j = 0; j < m_cols; j++)
                matrix(i, j) = (*this)(j, i);
        return matrix;
    }

    void transpose()
    {
        MatrixDbl matrix(m_cols, m_rows, Type::NotInitialized);
        for(int i = 0; i < m_rows; i++)
            for(int j = 0; j < m_cols; j++)
                matrix(i, j) = (*this)(j, i);

        m_rows = matrix.rows();
        m_cols = matrix.cols();
        for(int i = 0; i < m_rows * m_cols; i++)
            m_value[i] = matrix[i];
    }

    int cols() const
    {
        return m_cols;
    }

    int rows() const
    {
        return m_rows;
    }

    double *value() const
    {
        return m_value;
    }

protected:
    double *m_value = nullptr;
    int m_cols = 0;
    int m_rows = 0;
};

#endif // MATRIXDBL_H
