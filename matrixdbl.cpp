#include "matrixdbl.h"


int MatrixDbl::cols() const
{
    return m_cols;
}

int MatrixDbl::rows() const
{
    return m_rows;
}

double *MatrixDbl::value() const
{
    return m_value;
}
