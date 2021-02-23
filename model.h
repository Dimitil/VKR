#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QColor>
#include <QDebug>
#include <QSet>
#include <QQueue>

#include "cell.h"

using Gridtype = QVector<QVector<Cell>>;

class Model : public QObject
{
    Q_OBJECT

    Gridtype _grid;
    int _col;
    int _row;
    int _equalCount = 5;
    int _score = 0;
    int _fromCol;
    int _fromRow;
    int _toCol;
    int _toRow;

    bool (Model::*_algorithm)();
    void setAlgorithm(bool (Model::*algorithm)())
    {
        _algorithm = algorithm;
    }
    void horizontalCheck(QSet<Cell*> &set, int row, int col);
    void verticalCheck(QSet<Cell*> &set, int row, int col);
    void rightDiagonalCheck(QSet<Cell*> &set, int row, int col);
    void leftDiagonalCheck(QSet<Cell*> &set, int row, int col);
    int deleteSet(QSet<Cell*> &set);
    void addScore(int score);
    void coordinateAllCells();
    bool moveTo(int oldRow, int oldCol, int newRow, int newCol);
    void clear();
    void clearVisited();
    void addNeighbors(Cell* cell, QQueue<Cell*> &q);
    bool bfs(); //pathfinding algorithm

public:

    Model(int col = 10, int row = 10, QObject *parent = nullptr);

    Gridtype& getGrid();
    void addRandomFigures(int num);
    void addFigures(int row, int col, FigureType figureType);
    int checkAndDeleteLines(int row, int col);
    void resize(int row, int col);
    int fromRow() const;
    int fromCol() const;
    int toRow() const;
    int toCol() const;
    int row() const;
    int col() const;
    void setFrom(int row, int col);
    void setTo(int row, int col);
    bool doStep();

signals:
    void scoreChanged(int newScore);
};

#endif // MODEL_H
