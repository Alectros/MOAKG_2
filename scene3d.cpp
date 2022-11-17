#include "scene3d.h"

#include <cmath>

#include <QFormLayout>
#include <QLabel>
#include <QPainter>
#include <QResizeEvent>

Scene3D::Scene3D(QWidget *parent) : QDialog(parent)
{
    resize({m_camera.width(), m_camera.height()});
    m_actionMoveLeft = new QAction;
    m_actionMoveLeft->setShortcut(Qt::Key_A);
    addAction(m_actionMoveLeft);
    m_actionMoveRight = new QAction;
    m_actionMoveRight->setShortcut(Qt::Key_D);
    addAction(m_actionMoveRight);
    m_actionMoveForward = new QAction;
    m_actionMoveForward->setShortcut(Qt::Key_W);
    addAction(m_actionMoveForward);
    m_actionMoveBack = new QAction;
    m_actionMoveBack->setShortcut(Qt::Key_S);
    addAction(m_actionMoveBack);
    m_actionMoveUp = new QAction;
    m_actionMoveUp->setShortcut(Qt::Key_R);
    addAction(m_actionMoveUp);
    m_actionMoveDown = new QAction;
    m_actionMoveDown->setShortcut(Qt::Key_F);
    addAction(m_actionMoveDown);

    QFormLayout *layout = new QFormLayout;
    layout->addRow("Mouse wheel rotation", new QLabel("Camera focus changing"));
    layout->addRow("W, S", new QLabel("Move camera Forward or Back"));
    layout->addRow("A, D", new QLabel("Move camera Left or Right"));
    layout->addRow("R, F", new QLabel("Move camera Up or Down"));

    setLayout(layout);

    connect(m_actionMoveLeft, &QAction::triggered, this, &Scene3D::moveCameraController);
    connect(m_actionMoveRight, &QAction::triggered, this, &Scene3D::moveCameraController);
    connect(m_actionMoveForward, &QAction::triggered, this, &Scene3D::moveCameraController);
    connect(m_actionMoveBack, &QAction::triggered, this, &Scene3D::moveCameraController);
    connect(m_actionMoveUp, &QAction::triggered, this, &Scene3D::moveCameraController);
    connect(m_actionMoveDown, &QAction::triggered, this, &Scene3D::moveCameraController);
}

void Scene3D::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);

    for(int edgeInd = 0; edgeInd < m_model.edgesSize(); edgeInd++) {
        const int pointInd1 = m_model.edge(edgeInd).first;
        const int pointInd2 = m_model.edge(edgeInd).second;
        const VectorDbl4 pPxl1 = m_camera.worldToWindow(m_model.transformedPoint(pointInd1));
        const VectorDbl4 pPxl2 = m_camera.worldToWindow(m_model.transformedPoint(pointInd2));
        painter.drawLine(pPxl1.x(), pPxl1.y(), pPxl2.x(), pPxl2.y());
    }
}

void Scene3D::resizeEvent(QResizeEvent *event)
{
    const QSize newSize = event->size();
    m_camera.setHeight(newSize.height());
    m_camera.setWidth(newSize.width());
}

void Scene3D::wheelEvent(QWheelEvent *event)
{
    const double relative = (1 - event->angleDelta().y() / (1200.0));
    m_camera.setF(m_camera.F() * relative);
    update();
    event->accept();
}

void Scene3D::mousePressEvent(QMouseEvent *event)
{
    m_isMoving = true;
    m_lastPos = event->pos();
}

void Scene3D::mouseReleaseEvent(QMouseEvent *event)
{
    m_isMoving = false;
    m_lastPos = event->pos();
}

void Scene3D::mouseMoveEvent(QMouseEvent *event)
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
//    for(int i =0; i < m_models.size(); i++)
//        m_models[i].addTransform(MatrixDbl3x3::translation(v.x(), v.y()));

    m_lastPos = event->pos();
    update();
}

void Scene3D::clear()
{
    m_model.clear();
}

void Scene3D::setModel(const ModelShell3D &model)
{
    m_model = model;
}

Camera3D Scene3D::camera() const
{
    return m_camera;
}

void Scene3D::setCamera(const Camera3D &camera)
{
    m_camera = camera;
}

void Scene3D::moveCameraController()
{
    const double worldMove = 1e-1;
    if (sender() == m_actionMoveRight)
        moveCamera(worldMove, 0 ,0);
    if (sender() == m_actionMoveLeft)
        moveCamera(-worldMove, 0 ,0);
    if (sender() == m_actionMoveForward)
        moveCamera(0, 0, worldMove);
    if (sender() == m_actionMoveBack)
        moveCamera(0, 0, -worldMove);
    if (sender() == m_actionMoveUp)
        moveCamera(0, worldMove, 0);
    if (sender() == m_actionMoveDown)
        moveCamera(0, -worldMove, 0);
    update();
}

void Scene3D::moveCamera(const double x, const double y, const double z)
{
    m_camera.addPosition({x, y, z});
}

