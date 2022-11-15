#ifndef CAMERA3D_H
#define CAMERA3D_H

#include "camera2d.h"
#include "affinetools3d.h"

class Camera3D : public Camera2D
{
public:
    Camera3D() = default;

    void setL(const double L) override;
    void setR(const double R) override;
    void setB(const double B) override;
    void setT(const double T) override;

    enum class ProjectionType
    {
        Orthografic = 0,
        Perspective = 1
    };

    const MatrixDbl4x4& projection() const;
    const MatrixDbl4x4& view() const;

    double F() const;
    void setF(const double F);

    double D() const;
    void setD(const double D);

    ProjectionType projectionType() const;
    void setProjectionType(const ProjectionType &projectionType);

    VectorDbl3 eye() const;
    void setEye(const VectorDbl3 &eye);

    VectorDbl3 vWorldUp() const;
    void setVWorldUp(const VectorDbl3 &vWorldUp);

    VectorDbl4 position() const;
    void setPosition(const VectorDbl4 &position);

protected:
    MatrixDbl4x4 m_projection;
    ProjectionType m_projectionType = ProjectionType::Perspective;
    double m_F = 1e2;
    double m_D = 1e-1;

    MatrixDbl4x4 m_view;
    VectorDbl4 m_position;

    VectorDbl3 m_vWorldUp = {0, 1, 0};
    VectorDbl3 m_eye = {0, 0, -1};

    void recalculateView();

    void recalculatePerspectiveMatrix();
    void recalculateOrtograficMatrix();

    void recalculateProjectWithNewLR();
    void recalculateProjectWithNewTB();
    void recalculateProjectWithNewF();
    void recalculateProjectWithNewD();

};

#endif // CAMERA3D_H
