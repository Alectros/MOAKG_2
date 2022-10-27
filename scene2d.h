#ifndef SCENE2D_H
#define SCENE2D_H

#include <QWidget>

#include <QDialog>
#include <vector>

#include "camera2d.h"
#include "modelshell.h"

class Scene2D : public QDialog
{
    Q_OBJECT
public:
    Scene2D(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void clear();
    bool addModel(ModelShell* model);
    Camera2D camera() const;
    void setCamera(const Camera2D &camera);

private:
    Camera2D m_camera;
    std::vector<ModelShell*> m_models;
    bool m_isMoving = false;
    QPoint m_lastPos;
    ModelShell m_axes;
    void updateAxes();
};

#endif // SCENE2D_H
