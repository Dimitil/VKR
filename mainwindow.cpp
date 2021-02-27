#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include "model.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto model = new Model(this);
    ui->scene->setModel(model);

    connect(model, &Model::scoreChanged, ui->scoreDisplay,  QOverload<int>::of(&QLCDNumber::display));
    connect(ui->bfsPushButton, SIGNAL(clicked()), model, SLOT(setBfsAlgorithm()));
    connect(ui->bestFirstPushButton, SIGNAL(clicked()), model, SLOT(setBestFirstAlgorithm()));
    connect(ui->aStarPushButton, SIGNAL(clicked()), model, SLOT(setAStarAlgorithm()));
    connect(ui->eazyPushButton, SIGNAL(clicked()), model, SLOT(setEazyDifficulty()));
    connect(ui->normalPushButton, SIGNAL(clicked()), model, SLOT(setNormalDifficulty()));
    connect(ui->hardPushButton, SIGNAL(clicked()), model, SLOT(setHardDifficulty()));
    connect(model, SIGNAL(difficultyChanged()), ui->scene, SLOT(setFixedSize()));

    setFixedSize(minimumSize());
    emit model->difficultyChanged();
}

MainWindow::~MainWindow()
{
    delete ui;
}

