#ifndef SCENE3D_H
#define SCENE3D_H

#include <vector>

#include <QWidget>
#include <QDialog>
#include <QAction>

#include "camera3d.h"
#include "modelshell3d.h"

class Scene3D : public QDialog
{
    Q_OBJECT
public:
    Scene3D(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent*) override;

    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void clear();
    void setModel(const ModelShell3D &model);
    Camera3D camera() const;
    void setCamera(const Camera3D &camera);

private:
    Camera3D m_camera;
    ModelShell3D m_model;
    bool m_isMoving = false;
    QPoint m_lastPos;

    QAction *m_actionMoveLeft = nullptr;
    QAction *m_actionMoveRight = nullptr;
    QAction *m_actionMoveForward = nullptr;
    QAction *m_actionMoveBack = nullptr;
    QAction *m_actionMoveUp = nullptr;
    QAction *m_actionMoveDown = nullptr;

    void moveCameraController();
    void moveCamera(const double x, const double y, const double z);
};

#endif // SCENE3D_H
