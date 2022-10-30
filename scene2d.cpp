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
    QPen pen(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);

    for(int modelInd = 0; modelInd < m_models.size(); modelInd++) {
        for(int edgeInd = 0; edgeInd < m_models[modelInd].edgesSize(); edgeInd++) {
            const int pointInd1 = m_models[modelInd].edge(edgeInd).first;
            const int pointInd2 = m_models[modelInd].edge(edgeInd).second;
            const VectorDbl3 pPxl1 = m_camera.worldToPxl(m_models[modelInd].transformedPoint(pointInd1));
            const VectorDbl3 pPxl2 = m_camera.worldToPxl(m_models[modelInd].transformedPoint(pointInd2));
            painter.drawLine(pPxl1.x(), pPxl1.y(), pPxl2.x(), pPxl2.y());
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
    m_camera.m_T /= relativeAr;
    m_camera.m_B /= relativeAr;
}

void Scene2D::wheelEvent(QWheelEvent *event)
{
    const double relative = 1 - event->angleDelta().y() / (1200.0);
    for(int i =0; i < m_models.size(); i++)
        m_models[i].addTransform(MatrixDbl3x3::scale(relative, relative));
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

    VectorDbl3 v = m_camera.pxlToWorld(
                {static_cast<double>(currentPos.x()),
                 static_cast<double>(currentPos.y()),
                 1.0}
                ) - m_camera.pxlToWorld(
                {static_cast<double>(m_lastPos.x()),
                 static_cast<double>(m_lastPos.y()),
                 1.0}
                );
    v[2] = 1;
    for(int i =0; i < m_models.size(); i++)
        m_models[i].addTransform(MatrixDbl3x3::translation(v.x(), v.y()));

    m_lastPos = event->pos();
    update();
}

void Scene2D::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Minus) {
        int angle = 10;
        if (event->key() == Qt::Key_Minus)
            angle *= -1;
        for(int i =0; i < m_models.size(); i++) {
            const VectorDbl3 middlePoint = m_models[i].transform() * m_models[i].middlePoint();
            m_models[i].addTransform(
                        MatrixDbl3x3::translation(middlePoint.x(), middlePoint.y())
                    * MatrixDbl3x3::rotation(angle)
                    * MatrixDbl3x3::translation(-middlePoint.x(), -middlePoint.y()));
        }
        update();
    }
}

void Scene2D::clear()
{
    m_models.clear();
}

void Scene2D::addModel(const ModelShell &model)
{
    if (m_models.size() == 0)
        m_models.push_back(model);
    else
        m_models[0] = model;
}

Camera2D Scene2D::camera() const
{
    return m_camera;
}

void Scene2D::setCamera(const Camera2D &camera)
{
    m_camera = camera;
}
