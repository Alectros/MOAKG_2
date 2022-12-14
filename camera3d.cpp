#include "camera3d.h"

Camera3D::Camera3D()
{
    recalculatePerspectiveMatrix();
    recalculateView();
}

VectorDbl4 Camera3D::worldToView(const VectorDbl4 &world) const
{
    return m_view * (world - m_position);
}

VectorDbl4 Camera3D::viewToClip(const VectorDbl4 &view) const
{
    return m_projection * view;
}

VectorDbl4 Camera3D::clipToWindow(const VectorDbl4 &clip) const
{
    const double x = clip.x() * m_width / 2.0 + m_width / 2.0;
    const double y = clip.y() * m_height / 2.0 + m_height / 2.0;
    return {x, y, 0.0, 1.0};
}

VectorDbl4 Camera3D::worldToWindow(const VectorDbl4 &world) const
{
    return clipToWindow(viewToClip(worldToView(world)));
}

void Camera3D::setL(const double L)
{
    m_L = L;
    recalculateProjectWithNewLR();
}

void Camera3D::setR(const double R)
{
    m_R = R;
    recalculateProjectWithNewLR();
}

void Camera3D::setB(const double B)
{
    m_B = B;
    recalculateProjectWithNewTB();
}

void Camera3D::setT(const double T)
{
    m_T = T;
    recalculateProjectWithNewTB();
}

const MatrixDbl4x4 &Camera3D::projection() const
{
    return m_projection;
}

const MatrixDbl4x4 &Camera3D::view() const
{
    return m_view;
}

void Camera3D::recalculatePerspectiveMatrix()
{
    m_projection = {
        2.0 / (m_R - m_L), 0, (m_L + m_R) / ((m_R - m_L) * m_F), (m_L + m_R) / (m_L - m_R),
        0, 2.0 / (m_T - m_B), (m_B + m_T) / ((m_T - m_B) * m_F), (m_B + m_T) / (m_B - m_T),
        0, 0, -1.0 * (2.0 * m_F + m_D) / (m_D * m_F), -1,
        0, 0, -1.0 / m_F, 1
    };
}

void Camera3D::recalculateOrtograficMatrix()
{
    m_projection = {
        2.0 / (m_R - m_L), 0, 0, (m_L + m_R) / (m_L - m_R),
        0, 2.0 / (m_T - m_B), 0, (m_B + m_T) / (m_B - m_T),
        0, 0, -2.0 / m_D , -1,
        0, 0, 0, 1
    };
}

void Camera3D::recalculateProjectWithNewLR()
{
    m_projection(0, 0) = 2.0 / (m_R - m_L);
    m_projection(0, 3) = (m_L + m_R) / (m_L - m_R);
    if (m_projectionType == ProjectionType::Perspective)
        m_projection(0, 2) = (m_L + m_R) / ((m_R - m_L) * m_F);
}

void Camera3D::recalculateProjectWithNewTB()
{
    m_projection(1, 1) = 2.0 / (m_R - m_L);
    m_projection(1, 3) = (m_B + m_T) / (m_B - m_T);
    if (m_projectionType == ProjectionType::Perspective)
        m_projection(1, 2) = (m_B + m_T) / ((m_T - m_B) * m_F);
}

void Camera3D::recalculateProjectWithNewF()
{
    if (m_projectionType != ProjectionType::Perspective)
        return;
    m_projection(0, 2) = (m_L + m_R) / ((m_R - m_L) * m_F);
    m_projection(1, 2) = (m_B + m_T) / ((m_T - m_B) * m_F);
    m_projection(2, 2) = -1.0 * (2.0 * m_F + m_D) / (m_D * m_F);
    m_projection(3, 2) = -1.0 / m_F;
}

void Camera3D::recalculateProjectWithNewD()
{
    if (m_projectionType == ProjectionType::Perspective)
        m_projection(2, 2) = -1.0 * (2.0 * m_F + m_D) / (m_D * m_F);
    else
        m_projection(2, 2) = -2.0 / m_D;
}

double Camera3D::D() const
{
    return m_D;
}

void Camera3D::setD(const double D)
{
    m_D = D;
    recalculateProjectWithNewD();
}

Camera3D::ProjectionType Camera3D::projectionType() const
{
    return m_projectionType;
}

void Camera3D::setProjectionType(const ProjectionType &projectionType)
{
    if (m_projectionType == projectionType)
        return;
    m_projectionType = projectionType;
    if (m_projectionType == ProjectionType::Perspective)
        recalculatePerspectiveMatrix();
    else
        recalculateOrtograficMatrix();
}

VectorDbl4 Camera3D::eye() const
{
    return {m_eye[0], m_eye[1], m_eye[2], 1};
}

void Camera3D::setEye(const VectorDbl3 &eye)
{
    m_eye = eye;
    m_eye.normilize();
    recalculateView();
}

VectorDbl3 Camera3D::vWorldUp() const
{
    return m_vWorldUp;
}

void Camera3D::setVWorldUp(const VectorDbl3 &vWorldUp)
{
    m_vWorldUp = vWorldUp;
    recalculateView();
}

VectorDbl4 Camera3D::position() const
{
    return m_position;
}

void Camera3D::setPosition(const VectorDbl4 &position)
{
    m_position = position;
}

void Camera3D::addPosition(const VectorDbl3 &move)
{
    for(int i = 0; i < 3; i++)
        m_position[i] += move[i];
}

VectorDbl3 Camera3D::eylers() const
{
    return m_eylers;
}

void Camera3D::setEylers(const VectorDbl3 &eylers)
{
    m_eylers = eylers;
}

void Camera3D::recalculateView()
{
    m_view = MatrixDbl4x4::view(m_eye, m_vWorldUp);
}

double Camera3D::F() const
{
    return m_F;
}

void Camera3D::setF(const double F)
{
    m_F = F;
    recalculateProjectWithNewF();
}
