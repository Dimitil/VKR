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
    QAction *case2 = ui->menubar->addAction("Case 2");
    connect(case2, SIGNAL(triggered()), model, SLOT(testCase2()));
    QAction *case3 = ui->menubar->addAction("Case 3");
    connect(case3, SIGNAL(triggered()), model, SLOT(testCase3()));
    QAction *case4 = ui->menubar->addAction("Case 4");
    connect(case4, SIGNAL(triggered()), model, SLOT(testCase4()));
    QAction *autoTest = ui->menubar->addAction("AutoTest");
    connect(autoTest, SIGNAL(triggered()), model, SLOT(autoTest()));


    setFixedSize(minimumSize());
    emit model->difficultyChanged();
}

MainWindow::~MainWindow()
{
    delete ui;
}

