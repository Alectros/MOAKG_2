#include "camera2d.h"

Camera2D::Camera2D()
{

}

VectorDbl3 Camera2D::worldToPxl(const VectorDbl3 &xW) const
{
    const double xP = (xW[0] / xW[2] - m_L) / (m_R - m_L) * m_width;
    const double yP = (m_T - xW[1] / xW[2]) / (m_T - m_B) * m_height;
    return {xP, yP, 1};
}

VectorDbl3 Camera2D::pxlToWorld(const VectorDbl3 &xP) const
{
    const double xW = m_L + (m_R - m_L) * (xP[0] + 0.5) / m_width;
    const double yW = m_T - (m_T - m_B) * (xP[1] + 0.5) / m_height;
    return {xW, yW, 1};
}

int Camera2D::width() const
{
    return m_width;
}

int Camera2D::height() const
{
    return m_height;
}

void Camera2D::setWidth(const int width)
{
    m_width = width;
    setL(-initialWorldSide * m_width / m_height);
    setR(initialWorldSide * m_width / m_height);
}

void Camera2D::setHeight(const int height)
{
    m_height = height;
}

double Camera2D::L() const
{
    return m_L;
}

void Camera2D::setL(const double L)
{
    m_L = L;
}

double Camera2D::R() const
{
    return m_R;
}

void Camera2D::setR(const double R)
{
    m_R = R;
}

double Camera2D::B() const
{
    return m_B;
}

void Camera2D::setB(const double B)
{
    m_B = B;
}

double Camera2D::T() const
{
    return m_T;
}

void Camera2D::setT(const double T)
{
    m_T = T;
}
