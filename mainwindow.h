#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "scene2d.h"
#include "modelshell3d.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void execDialog();
    void importModel();

private:
    Ui::MainWindow *ui;
    Scene2D *sceneDialog = new Scene2D(this);

};
#endif // MAINWINDOW_H
