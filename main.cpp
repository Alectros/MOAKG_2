#include "mainwindow.h"

#include <QApplication>

#include "vectordbl.h"

int main(int argc, char *argv[])
{
    MatrixDbl m1(3, 3);
    VectorDbl v1(3);
    m1 * v1;
    v1 * m1;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
