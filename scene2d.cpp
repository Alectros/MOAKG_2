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
    const double relativeAr = (static_cast<double>(newSize.width()) * event->oldSize().height())
            / (static_cast<double>(newSize.height()) * event->oldSize().width());
    m_camera.m_height = newSize.height();
    m_camera.m_width = newSize.width();
    m_camera.m_R *= relativeAr;
    m_camera.m_L *= relativeAr;
}

void Scene2D::wheelEvent(QWheelEvent *event)
{
    const double relative = 1 - event->angleDelta().y() / (1200.0);
    for(int i =0; i < m_models.size(); i++)
        m_models[i]->addTransform(MatrixDbl3x3::scale(relative, relative));
    update();
    event->accept();
}

void Scene2D::mousePressEvent(QMouseEvent *event)
{
    m_isMoving = true;
    m_lastPos = event->pos();
}

void Scene2D::mouseReleaseEvent(QMouseEvent *event)
{
    m_isMoving = false;
    m_lastPos = event->pos();
}

void Scene2D::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_isMoving)
        return;
    const QPoint currentPos = event->pos();

    const VectorDbl3 v = m_camera.pxlToWorld(
                {static_cast<double>(currentPos.x()),
                 static_cast<double>(currentPos.y()),
                 1.0}
                ) - m_camera.pxlToWorld(
                {static_cast<double>(m_lastPos.x()),
                 static_cast<double>(m_lastPos.y()),
                 1.0}
                );

    for(int i =0; i < m_models.size(); i++)
        m_models[i]->addTransform(MatrixDbl3x3::translation(v[0], v[1]));

    m_lastPos = event->pos();
    update();
}

void Scene2D::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Minus) {
        int angle = 10;
        if (event->key() == Qt::Key_Minus)
            angle *= -1;
        for(int i =0; i < m_models.size(); i++)
            m_models[i]->addTransform(MatrixDbl3x3::rotation(angle));
        update();
    }
}

void Scene2D::clear()
{
    m_models.clear();
}

bool Scene2D::addModel(ModelShell *model)
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
