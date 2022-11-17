#ifndef MODELSHELLIMPORT_H
#define MODELSHELLIMPORT_H

#include <QString>
#include <QFile>
#include <QTextStream>

#include "modelshell3d.h"


#include <QString>
#include <QFile>
#include <QTextStream>

#include "modelshell3d.h"

//Структура файла - File structure
//количество точек - Points count
//точки с 4-мя координатами - Points with 4 coordinates
//количество ребер - Edges count
//ребра (нумерация с 1) - Edges (begins from 1)
//Пример - Example
//4
//0 0 0 1
//1 0 0 1
//0 1 1 1
//0 1 0 1
//3
//1 2
//2 3
//3 4

bool importModelShellFromFile(const QString &filename, ModelShell3D &model, QString &errors)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errors = "File not open";
        return false;
    }
    QTextStream stream(&file);
    int sP, sE;
    stream >> sP;
    model.clear();
    for(int i = 0; i < sP; i++) {
        double x, y, z, w;
        stream >> x >> y >> z >> w;
        model.addPoint(x / w, y / w, z / w);
    }
    stream >> sE;
    for(int i = 0; i < sE; i++) {
        double p1, p2;
        stream >> p1 >> p2;
        model.addEdge(p1 - 1, p2 - 1);
    }
    file.close();
    return true;
}


#endif // MODELSHELLIMPORT_H
