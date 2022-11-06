#include "modelshell3d.h"

void ModelShell3D::clear()
{
    m_points.clear();
    m_edges.clear();
}

void ModelShell3D::addPoint(const double x, const double y, const double z)
{
    m_points.push_back({x, y, z, 1});
    m_middlePoint = m_middlePoint + m_points[m_points.size() - 1];
}

bool ModelShell3D::addEdge(const int indE1, const int indE2)
{
    if (indE1 < 0 || indE2 < 0 || indE1 >= m_points.size() || indE2 >= m_points.size())
        return false;

    m_edges.push_back({indE1, indE2});
    return true;
}

std::vector<VectorDbl4> ModelShell3D::points() const
{
    return m_points;
}

VectorDbl4 ModelShell3D::point(const int ind) const
{
    assert(ind >= 0 && ind < m_points.size());
    return m_points[ind];
}

VectorDbl4 ModelShell3D::transformedPoint(const int ind) const
{
    assert(ind >= 0 && ind < m_points.size());
    return m_transform * m_points[ind];
}

VectorDbl4 &ModelShell3D::point(const int ind)
{
    assert(ind >= 0 && ind < m_points.size());
    return m_points[ind];
}

int ModelShell3D::pointsSize() const
{
    return m_points.size();
}

std::vector<std::pair<int, int> > ModelShell3D::edges() const
{
    return m_edges;
}

std::pair<int, int> ModelShell3D::edge(const int ind) const
{
    assert(ind >= 0 && ind < m_edges.size());
    return m_edges[ind];
}

std::pair<int, int> &ModelShell3D::edge(const int ind)
{
    assert(ind >= 0 && ind < m_edges.size());
    return m_edges[ind];
}

int ModelShell3D::edgesSize() const
{
    return m_edges.size();
}

void ModelShell3D::addTransform(const MatrixDbl4x4 &matrix)
{
    m_transform = matrix * m_transform;
}

void ModelShell3D::applyTransform()
{
    for(int i = 0; i < m_points.size(); i++)
        m_points[i] = m_transform * m_points[i];
}

MatrixDbl4x4 ModelShell3D::transform() const
{
    return m_transform;
}

void ModelShell3D::clearTransform()
{
    m_transform = MatrixDbl4x4();
}

VectorDbl4 ModelShell3D::middlePoint() const
{
    if (m_points.size() == 0)
        return VectorDbl4();
    return VectorDbl4(m_middlePoint.x() / m_points.size(), m_middlePoint.y() / m_points.size(), m_middlePoint.z() / m_points.size(), 1);
}
