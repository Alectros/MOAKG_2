#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "matrixdbl3x3.h"

class Camera2D
{
public:
    Camera2D();

    VectorDbl3 worldToPxl(const VectorDbl3 &xW) const;
    VectorDbl3 pxlToWorld(const VectorDbl3 &xP) const;

    int m_width = 700;
    int m_height = 700;
    int m_offsetX = 0;
    int m_offsetY = 0;
    double m_L = -1;
    double m_R = 1;
    double m_B = -1;
    double m_T = 1;
};

#endif // CAMERA2D_H
