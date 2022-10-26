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

    void clear();
    bool addModel(const ModelShell* model);
    Camera2D camera() const;
    void setCamera(const Camera2D &camera);

private:
    Camera2D m_camera;
    std::vector<const ModelShell*> m_models;

};

#endif // SCENE2D_H
