#include "modelshell.h"

ModelShell::ModelShell()
{

}

void ModelShell::clear()
{
    m_points.clear();
    m_edges.clear();
}

void ModelShell::addPoint(const double x, const double y)
{
    m_points.push_back({x, y, 1});
}

bool ModelShell::addEdge(const int indE1, const int indE2)
{
    if (indE1 < 0 || indE2 < 0 || indE1 >= m_points.size() || indE2 >= m_points.size())
        return false;

    m_edges.push_back({indE1, indE2});
    return true;
}

std::vector<VectorDbl3> ModelShell::points() const
{
    return m_points;
}

VectorDbl ModelShell::point(const int ind) const
{
    assert(ind >= 0 && ind < m_points.size());
    return m_points[ind];
}

VectorDbl &ModelShell::point(const int ind)
{
    assert(ind >= 0 && ind < m_points.size());
    return m_points[ind];
}

int ModelShell::pointsSize() const
{
    return m_points.size();
}

std::vector<std::pair<int, int> > ModelShell::edges() const
{
    return m_edges;
}

std::pair<int, int> ModelShell::edge(const int ind) const
{
    assert(ind >= 0 && ind < m_edges.size());
    return m_edges[ind];
}

std::pair<int, int> &ModelShell::edge(const int ind)
{
    assert(ind >= 0 && ind < m_edges.size());
    return m_edges[ind];
}

int ModelShell::edgesSize() const
{
    return m_edges.size();
}

void ModelShell::applyTransform(const MatrixDbl3x3 &matrix)
{
    for(int i = 0; i < m_points.size(); i++)
        m_points[i] = matrix * m_points[i];
}
