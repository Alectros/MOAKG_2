QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera2d.cpp \
    main.cpp \
    mainwindow.cpp \
    matrixdbl.cpp \
    matrixdbl3x3.cpp \
    modelshell.cpp \
    scene2d.cpp \
    vectordbl.cpp

HEADERS += \
    camera2d.h \
    mainwindow.h \
    matrixdbl.h \
    matrixdbl3x3.h \
    modelShellImport.h \
    modelshell.h \
    scene2d.h \
    vectordbl.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
