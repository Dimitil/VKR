#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include "model.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //auto view = new View();
    auto model = new Model(10, 10);


    ui->scene->setModel(model);
    ui->scene->setFixedSize(QSize(
                                model->getGrid().size()*50+1, model->getGrid()[0].size()*50+1)); //FIXME

    //model->addRandomFigures(3);       //debug

    model->addFigures(0,0,0);           //debug
    model->addFigures(0,1,1);           //debug
    model->addFigures(0,2,2);           //debug
    model->addFigures(0,3,3);           //debug
    model->addFigures(0,4,4);           //debug
}

MainWindow::~MainWindow()
{
    delete ui;
}

