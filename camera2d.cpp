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
