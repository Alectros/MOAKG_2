#ifndef SCENE2D_H
#define SCENE2D_H

#include <vector>

#include <QWidget>
#include <QDialog>
#include <QAction>

#include "camera2d.h"
#include "modelshell3d.h"

class Scene2D : public QDialog
{
    Q_OBJECT
public:
    Scene2D(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent*) override;

    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void clear();
    void addModel(const ModelShell3D &model);
    Camera2D camera() const;
    void setCamera(const Camera2D &camera);

private:
    Camera2D m_camera;
    std::vector<ModelShell3D> m_models;
    bool m_isMoving = false;
    QPoint m_lastPos;

    QAction *m_actionRotationLeft = nullptr;
    QAction *m_actionRotationRight = nullptr;
    QAction *m_actionRelativeScaleXPos = nullptr;
    QAction *m_actionRelativeScaleYPos = nullptr;
    QAction *m_actionRelativeScaleXNeg = nullptr;
    QAction *m_actionRelativeScaleYNeg = nullptr;

    void rotateFigureLeft();
    void rotateFigureRight();
    void rotateFigure(const double &angle);
    void scaleRelativeController();
    void scaleRelativeFirstEdge(const double coeffX, const double coeffY);
};

#endif // SCENE2D_H
