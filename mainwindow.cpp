#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "modelShellImport.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->buttonRun, &QPushButton::clicked, this, &MainWindow::execDialog);
    connect(ui->buttonImportModel, &QPushButton::clicked, this, &MainWindow::importModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::execDialog()
{
    sceneDialog->exec();
}

void MainWindow::importModel()
{
    const QString filename = QFileDialog::getOpenFileName(this);
    if (filename.isEmpty()) {
        QMessageBox::warning(this, "Import Error", "Filename is empty");
        return;
    }

    QString errors;
    ModelShell model;
    if (!importModelShellFromFile(filename, model, errors)) {
        QMessageBox::warning(this, "Import Error", "Couldn't import model");
        return;
    }
    models.push_back(model);
    sceneDialog->addModel(&models[models.size() - 1]);
    ui->labelmModelName->setText(filename);
}

