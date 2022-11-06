#ifndef MODELSHELL3D_H
#define MODELSHELL3D_H

#include "affinetools3d.h"
#include <vector>
#include <string>

class ModelShell3D
{
public:
    ModelShell3D() = default;

    void clear();
    void addPoint(const double x, const double y, const double z);
    bool addEdge(const int indE1, const int indE2);

    std::vector<VectorDbl4> points() const;
    VectorDbl4 point(const int ind) const;
    VectorDbl4 transformedPoint(const int ind) const;
    VectorDbl4& point(const int ind);
    int pointsSize() const;
    std::vector<std::pair<int, int> > edges() const;
    std::pair<int, int> edge(const int ind) const;
    std::pair<int, int>& edge(const int ind);
    int edgesSize() const;
    void addTransform(const MatrixDbl4x4 &matrix);
    void applyTransform();
    MatrixDbl4x4 transform() const;
    void clearTransform();
    VectorDbl4 middlePoint() const;
protected:
    std::vector<VectorDbl4> m_points;
    std::vector<std::pair<int, int>> m_edges;
    MatrixDbl4x4 m_transform;
    VectorDbl4 m_middlePoint;
};

#endif // MODELSHELL3D_H
