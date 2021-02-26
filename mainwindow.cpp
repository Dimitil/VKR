#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include "model.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto model = new Model(10, 10, this);


    ui->scene->setModel(model);
    ui->scene->setFixedSize(QSize(
                                model->getGrid().size()*50+1, model->getGrid()[0].size()*50+1)); //FIXME

    model->addRandomFigures(30);       //debug


    connect(model, &Model::scoreChanged, ui->scoreDisplay,  QOverload<int>::of(&QLCDNumber::display));
    connect(ui->bfsPushButton, SIGNAL(clicked()), model, SLOT(setBfsAlgorithm()));
    connect(ui->bestFirstPushButton, SIGNAL(clicked()), model, SLOT(setBestFirstAlgorithm()));
    connect(ui->aStarPushButton, SIGNAL(clicked()), model, SLOT(setAStarAlgorithm()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

