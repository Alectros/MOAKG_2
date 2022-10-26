#ifndef MODELSHELL_H
#define MODELSHELL_H

#include "matrixdbl3x3.h"
#include <vector>
#include <string>

class ModelShell
{
public:
    ModelShell();

    void clear();
    void addPoint(const double x, const double y);
    bool addEdge(const int indE1, const int indE2);

    std::vector<VectorDbl3> points() const;
    VectorDbl3 point(const int ind) const;
    VectorDbl3 transformedPoint(const int ind) const;
    VectorDbl3& point(const int ind);
    int pointsSize() const;
    std::vector<std::pair<int, int> > edges() const;
    std::pair<int, int> edge(const int ind) const;
    std::pair<int, int>& edge(const int ind);
    int edgesSize() const;
    void applyTransform(const MatrixDbl3x3 &matrix);
    MatrixDbl3x3 transform() const;
protected:
    std::vector<VectorDbl3> m_points;
    std::vector<std::pair<int, int>> m_edges;
    MatrixDbl3x3 m_transform;
};

#endif // MODELSHELL_H
