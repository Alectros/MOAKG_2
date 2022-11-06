#include "scene2d.h"

#include <cmath>

#include <QFormLayout>
#include <QLabel>
#include <QPainter>
#include <QResizeEvent>

Scene2D::Scene2D(QWidget *parent) : QDialog(parent)
{
    resize({m_camera.m_width, m_camera.m_height});
    m_actionRotationLeft = new QAction;
    m_actionRotationLeft->setShortcut(Qt::Key_Plus);
    addAction(m_actionRotationLeft);
    m_actionRotationRight = new QAction;
    m_actionRotationRight->setShortcut(Qt::Key_Minus);
    addAction(m_actionRotationRight);
    m_actionRelativeScaleXPos = new QAction;
    m_actionRelativeScaleXPos->setShortcut(Qt::Key_Right);
    addAction(m_actionRelativeScaleXPos);
    m_actionRelativeScaleYPos = new QAction;
    m_actionRelativeScaleYPos->setShortcut(Qt::Key_Up);
    addAction(m_actionRelativeScaleYPos);
    m_actionRelativeScaleXNeg = new QAction;
    m_actionRelativeScaleXNeg->setShortcut(Qt::Key_Left);
    addAction(m_actionRelativeScaleXNeg);
    m_actionRelativeScaleYNeg = new QAction;
    m_actionRelativeScaleYNeg->setShortcut(Qt::Key_Down);
    addAction(m_actionRelativeScaleYNeg);

    QFormLayout *layout = new QFormLayout;
    layout->addRow("Move the mouse with hold button", new QLabel("Dragging the object"));
    layout->addRow("Mouse wheel rotation", new QLabel("Scaling the object"));
    layout->addRow("+", new QLabel("Rotate object 10 degrees counterclockwise"));
    layout->addRow("-", new QLabel("Rotate object 10 degrees clockwise"));
    layout->addRow("UpArrow, DownArrow", new QLabel("Scale object along red edge"));
    layout->addRow("LeftArrow, RightArrow", new QLabel("Scale object perpendicular red edge"));

    setLayout(layout);

    connect(m_actionRotationLeft, &QAction::triggered, this, &Scene2D::rotateFigureLeft);
    connect(m_actionRotationRight, &QAction::triggered, this, &Scene2D::rotateFigureRight);
    connect(m_actionRelativeScaleXPos, &QAction::triggered, this, &Scene2D::scaleRelativeController);
    connect(m_actionRelativeScaleYPos, &QAction::triggered, this, &Scene2D::scaleRelativeController);
    connect(m_actionRelativeScaleXNeg, &QAction::triggered, this, &Scene2D::scaleRelativeController);
    connect(m_actionRelativeScaleYNeg, &QAction::triggered, this, &Scene2D::scaleRelativeController);
}

void Scene2D::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen(Qt::black);
    QPen penF(Qt::red);
    pen.setWidth(2);
    penF.setWidth(2);
    painter.setPen(penF);

    for(int modelInd = 0; modelInd < m_models.size(); modelInd++) {
        for(int edgeInd = 0; edgeInd < m_models[modelInd].edgesSize(); edgeInd++) {
            const int pointInd1 = m_models[modelInd].edge(edgeInd).first;
            const int pointInd2 = m_models[modelInd].edge(edgeInd).second;
            const VectorDbl3 pPxl1 = m_camera.worldToPxl(m_models[modelInd].transformedPoint(pointInd1));
            const VectorDbl3 pPxl2 = m_camera.worldToPxl(m_models[modelInd].transformedPoint(pointInd2));
            painter.drawLine(pPxl1.x(), pPxl1.y(), pPxl2.x(), pPxl2.y());
            if (edgeInd == 0)
                painter.setPen(pen);
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

void Scene2D::clear()
{
    m_models.clear();
}

void Scene2D::addModel(const ModelShell3D &model)
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

void Scene2D::rotateFigureLeft()
{
    rotateFigure(10);
}

void Scene2D::rotateFigureRight()
{
    rotateFigure(-10);
}

void Scene2D::rotateFigure(const double &angle)
{
    for(int i =0; i < m_models.size(); i++) {
        const VectorDbl3 middlePoint = m_models[i].transform() * m_models[i].middlePoint();
        m_models[i].addTransform(
                    MatrixDbl3x3::translation(middlePoint.x(), middlePoint.y())
                * MatrixDbl3x3::rotation(angle)
                * MatrixDbl3x3::translation(-middlePoint.x(), -middlePoint.y()));
    }
    update();
}

void Scene2D::scaleRelativeController()
{
    const double scaleFactor = 1.1;
    const QAction *sender = qobject_cast<QAction*>(QObject::sender());
    if (sender == nullptr)
        return;
    if (sender == m_actionRelativeScaleXPos)
        scaleRelativeFirstEdge(scaleFactor, 1);
    if (sender == m_actionRelativeScaleYPos)
        scaleRelativeFirstEdge(1, scaleFactor);
    if (sender == m_actionRelativeScaleXNeg)
        scaleRelativeFirstEdge(1 / scaleFactor, 1);
    if (sender == m_actionRelativeScaleYNeg)
        scaleRelativeFirstEdge(1, 1 / scaleFactor);
}

void Scene2D::scaleRelativeFirstEdge(const double coeffX, const double coeffY)
{
    if (m_models.size() == 0 || m_models[0].edgesSize() == 0)
        return;

    const int pointInd1 = m_models[0].edge(0).first;
    const int pointInd2 = m_models[0].edge(0).second;
    VectorDbl3 v = (m_models[0].transformedPoint(pointInd1) - m_models[0].transformedPoint(pointInd2));
    if (std::abs(v.length()) < std::numeric_limits<double>::epsilon())
        return;
    v.normilize();

    //! Нормализированный вектор может быть записан следующим образом
    //! v(sin(a), cos(a), 0), где a угол между осью y и гранью модели проходящей через начало координат
    //! Чтобы не тратить время на вычисления тангенса, а также косинусов и синусов при вызове MatrixDbl3x3::rotation(angle), записал их напрямую
    //!
    //! the normalized vector can be written as follows
    //! v(sin(a), cos(a), 0), where a is the angle between the y-axis and the first edge of the model passing through the origin
    //! So to prevent time consuming calculations of tg, cos and sin when calling MatrixDbl3x3::rotation(angle), i wrote them directly
    const MatrixDbl3x3 posRotate =
    {
        v[1], -v[0], 0,
        v[0],  v[1], 0,
           0,     0, 1
    },
            negRotate =
    {
        v[1],  v[0], 0,
       -v[0],  v[1], 0,
           0,     0, 1
    };

    const VectorDbl3 rPoint = (m_models[0].transformedPoint(pointInd1) + m_models[0].transformedPoint(pointInd2)) * 0.5;

    m_models[0].addTransform(
                MatrixDbl3x3::translation(rPoint.x(), rPoint.y())
            * negRotate
            * MatrixDbl3x3::scale(coeffX, coeffY)
            * posRotate
            * MatrixDbl3x3::translation(-rPoint.x(), -rPoint.y()));
    update();
}
