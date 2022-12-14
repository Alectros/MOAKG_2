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
    m_actionResetCamera = new QAction;
    m_actionResetCamera->setShortcut(Qt::Key_Home);
    addAction(m_actionResetCamera);
    m_actionRotateEdgeModelPls = new QAction;
    m_actionRotateEdgeModelPls->setShortcut(Qt::CTRL + Qt::Key_Plus);
    addAction(m_actionRotateEdgeModelPls);
    m_actionRotateEdgeModelNeg = new QAction;
    m_actionRotateEdgeModelNeg->setShortcut(Qt::CTRL + Qt::Key_Minus);
    addAction(m_actionRotateEdgeModelNeg);

    QFormLayout *layout = new QFormLayout;
    layout->addRow("Mouse wheel rotation", new QLabel("Camera focus changing"));
    layout->addRow("Mouse move", new QLabel("Camera Rotation"));
    layout->addRow("Home", new QLabel("Reset Camera"));
    layout->addRow("W, S", new QLabel("Move camera Forward or Back"));
    layout->addRow("A, D", new QLabel("Move camera Left or Right"));
    layout->addRow("R, F", new QLabel("Move camera Up or Down"));
    layout->addRow("CTRL +-", new QLabel("Rotate model around red edge"));

    setLayout(layout);

    connect(m_actionMoveLeft, &QAction::triggered, this, &Scene3D::moveCameraController);
    connect(m_actionMoveRight, &QAction::triggered, this, &Scene3D::moveCameraController);
    connect(m_actionMoveForward, &QAction::triggered, this, &Scene3D::moveCameraController);
    connect(m_actionMoveBack, &QAction::triggered, this, &Scene3D::moveCameraController);
    connect(m_actionMoveUp, &QAction::triggered, this, &Scene3D::moveCameraController);
    connect(m_actionMoveDown, &QAction::triggered, this, &Scene3D::moveCameraController);
    connect(m_actionResetCamera, &QAction::triggered, this, &Scene3D::resetCamera);
    connect(m_actionRotateEdgeModelPls, &QAction::triggered, this, &Scene3D::rotateAroundEdgeController);
    connect(m_actionRotateEdgeModelNeg, &QAction::triggered, this, &Scene3D::rotateAroundEdgeController);
}

void Scene3D::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen(Qt::black);
    pen.setWidth(2);
    QPen penRed(Qt::red);
    penRed.setWidth(2);
    painter.setPen(penRed);

    for(int edgeInd = 0; edgeInd < m_model.edgesSize(); edgeInd++) {
        const int pointInd1 = m_model.edge(edgeInd).first;
        const int pointInd2 = m_model.edge(edgeInd).second;
        const VectorDbl4 pPxl1 = m_camera.worldToWindow(m_model.transformedPoint(pointInd1));
        const VectorDbl4 pPxl2 = m_camera.worldToWindow(m_model.transformedPoint(pointInd2));
        painter.drawLine(pPxl1.x(), pPxl1.y(), pPxl2.x(), pPxl2.y());
        if (edgeInd == 0)
            painter.setPen(pen);
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
    m_firstPos = event->pos();
}

void Scene3D::mouseReleaseEvent(QMouseEvent *event)
{
    m_isMoving = false;
    m_firstPos = event->pos();
}

void Scene3D::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_isMoving)
        return;
    const QPoint currentPos = event->pos();
    const double staticAngle = 30;

    VectorDbl3 v = m_camera.pxlToWorld(
                {static_cast<double>(currentPos.x()),
                 static_cast<double>(currentPos.y()),
                 1.0}
                ) - m_camera.pxlToWorld(
                {static_cast<double>(m_firstPos.x()),
                 static_cast<double>(m_firstPos.y()),
                 1.0}
                );
    v[2] = 1;
    v = v * staticAngle;

    const VectorDbl4 camPos = m_camera.position();

    const VectorDbl4 currentEye =
            MatrixDbl4x4::translation(camPos[0], camPos[1], camPos[2])
            * MatrixDbl4x4::rotationY(v[0])
            * MatrixDbl4x4::rotationX(v[1])
            * MatrixDbl4x4::translation(-camPos[0], -camPos[1], -camPos[2])
            * m_camera.eye();
    m_camera.setEye(currentEye);
    m_firstPos = currentPos;
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

void Scene3D::resetCamera()
{
    m_camera = Camera3D();
    m_model.clearTransform();
    update();
}

void Scene3D::moveCameraController()
{
    const double worldMove = 1e-1;
    if (sender() == m_actionMoveRight)
        moveCamera(worldMove, 0 ,0);
    if (sender() == m_actionMoveLeft)
        moveCamera(-worldMove, 0 ,0);
    if (sender() == m_actionMoveForward)
        moveCamera(0, 0, -worldMove);
    if (sender() == m_actionMoveBack)
        moveCamera(0, 0, worldMove);
    if (sender() == m_actionMoveUp)
        moveCamera(0, -worldMove, 0);
    if (sender() == m_actionMoveDown)
        moveCamera(0, worldMove, 0);
    update();
}

void Scene3D::moveCamera(const double x, const double y, const double z)
{
    const VectorDbl4 pos = m_camera.view() * VectorDbl4(x, y, z, 1);
    m_camera.addPosition({pos.x(), pos.y(), pos.z()});
}

void Scene3D::rotateAroundEdgeController()
{
    const double rotateAngle = 10;
    if (sender() == m_actionRotateEdgeModelPls)
        rotateAroundEdge(rotateAngle);
    if (sender() == m_actionRotateEdgeModelNeg)
        rotateAroundEdge(-rotateAngle);
    update();
}

void Scene3D::rotateAroundEdge(const double &angle)
{
    if(m_model.edgesSize() == 0)
        return;
    const int pointInd1 = m_model.edge(0).first;
    const int pointInd2 = m_model.edge(0).second;
    const VectorDbl4 p1 = m_model.transformedPoint(pointInd1);
    const VectorDbl4 p2 = m_model.transformedPoint(pointInd2);
    const VectorDbl4 edgeVector = (p2 - p1);
    const VectorDbl3 normal = VectorDbl3(edgeVector[0], edgeVector[1], edgeVector[2]).normilized();

    //! ?????????????? ???????????? ?????????? ???????????????????????????? ??????????, ?????????? ????????????
    //! The rotation around the edge was carried out by changing the basis of the vectors

    VectorDbl3 worldVector = {0, 1, 0};
    if (std::abs(VectorDbl3::scalarImpl(worldVector, normal)) >= 1.0)
        worldVector = {1, 0, 0};
    const MatrixDbl4x4 view = MatrixDbl4x4::view(normal, worldVector);
    const MatrixDbl4x4 viewTransposed = MatrixDbl4x4(view.transposed());

    m_model.addTransform(
                MatrixDbl4x4::translation(p1.x(), p1.y(), p1.z())
                * viewTransposed
               * MatrixDbl4x4::rotationZ(angle)
                * view
               * MatrixDbl4x4::translation(-p1.x(), -p1.y(), -p1.z()));
}

