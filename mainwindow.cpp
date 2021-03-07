#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include "model.h"
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto model = new Model(this);
    ui->scene->setModel(model);

    setWindowTitle("Lines");

    connect(ui->bfsPushButton, &QPushButton::clicked, model, &Model::setBfsAlgorithm);
    connect(ui->bestFirstPushButton, &QPushButton::clicked, model, &Model::setBestFirstAlgorithm);
    connect(ui->aStarPushButton, &QPushButton::clicked, model, &Model::setAStarAlgorithm);
    connect(ui->eazyPushButton, &QPushButton::clicked, model, &Model::setEazyDifficulty);
    connect(ui->normalPushButton, &QPushButton::clicked, model, &Model::setNormalDifficulty);
    connect(ui->hardPushButton, &QPushButton::clicked, model, &Model::setHardDifficulty);
    connect(ui->extraHardPushButton, &QPushButton::clicked, model, &Model::setExtraHardDifficulty);
    connect(model, &Model::difficultyChanged, ui->scene, &View::setFixedSize);
    connect(model, &Model::gameOver, ui->scene, &View::gameOverDialog);
    connect(model, &Model::scoreChanged, ui->scoreDisplay,  QOverload<int>::of(&QLCDNumber::display));

    QAction *case1 = ui->menubar->addAction("Case 1");
    connect(case1, SIGNAL(triggered()), model, SLOT(testCase1()));

    setFixedSize(minimumSize());
    emit model->difficultyChanged();
}

MainWindow::~MainWindow()
{
    delete ui;
}

