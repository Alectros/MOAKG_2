#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "affinetools2d.h"

#define initialWorldSide 1.0

class Camera2D
{
public:
    Camera2D();

    VectorDbl3 worldToPxl(const VectorDbl3 &xW) const;
    VectorDbl3 pxlToWorld(const VectorDbl3 &xP) const;

    int width() const;
    int height() const;

    virtual void setWidth(const int width);
    virtual void setHeight(const int height);

    double L() const;
    double R() const;
    double B() const;
    double T() const;

    virtual void setL(const double L);
    virtual void setR(const double R);
    virtual void setB(const double B);
    virtual void setT(const double T);

protected:
    int m_width = 700;
    int m_height = 700;
    double m_L = -initialWorldSide;
    double m_R = initialWorldSide;
    double m_B = -initialWorldSide;
    double m_T = initialWorldSide;
};


#endif // CAMERA2D_H
