#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include "model.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto view = new View();
    auto model = new Model(10, 10);


    ui->scene->setModel(model);
    ui->scene->setFixedSize(QSize(
                                model->getGrid().size()*50+1, model->getGrid()[0].size()*50+1)); //FIXME

}

MainWindow::~MainWindow()
{
    delete ui;
}

