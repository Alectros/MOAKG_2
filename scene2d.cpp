#include "scene2d.h"

#include <QPainter>
#include <QResizeEvent>

Scene2D::Scene2D(QWidget *parent) : QDialog(parent)
{
    resize({m_camera.m_width, m_camera.m_height});
}

void Scene2D::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black));

    QPoint m_cursorPosition(0, 0);
    for(int modelInd = 0; modelInd < m_models.size(); modelInd++) {
        for(int edgeInd = 0; edgeInd < m_models[modelInd]->edgesSize(); edgeInd++) {
            const int edge1 = m_models[modelInd]->edge(edgeInd).first;
            const int edge2 = m_models[modelInd]->edge(edgeInd).second;
            auto pPxl1 = m_camera.worldToPxl(m_models[modelInd]->transformedPoint(edge1));
            auto pPxl2 = m_camera.worldToPxl(m_models[modelInd]->transformedPoint(edge2));
            painter.drawLine(pPxl1[0], pPxl1[1], pPxl2[0], pPxl2[1]);
        }
    }
}

void Scene2D::resizeEvent(QResizeEvent *event)
{
    const QSize newSize = event->size();
    m_camera.m_height = newSize.height();
    m_camera.m_width = newSize.width();
}

void Scene2D::clear()
{

}

bool Scene2D::addModel(const ModelShell *model)
{
    if (model == nullptr)
        return false;
    m_models.push_back(model);
    return true;
}

Camera2D Scene2D::camera() const
{
    return m_camera;
}

void Scene2D::setCamera(const Camera2D &camera)
{
    m_camera = camera;
}
