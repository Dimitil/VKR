#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QColor>
#include <QDebug>
#include <QSet>
#include <QQueue>
#include <queue>
#include "cell.h"

struct comparatorBestFirst{
    bool operator()(const Cell* l, const Cell* r)
    {
        return l->heuristic() > r->heuristic();
    };
};

struct comparatorAStar{
    bool operator()(const Cell* l, const Cell* r)
    {
        int lSum = l->heuristic() + l->moveCost();
        int rSum = r->heuristic() + r->moveCost();
        return lSum > rSum;
    }
};

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
    void clearMoveCost()
    {
        for (int r = 0; r < _row; r++)
        {
            for(int c = 0; c < _col; c++)
            {
                _grid[r][c].setMoveCost(99999);
            }
        }
    }
    void addNeighborsBFS(Cell* cell, QQueue<Cell*> &q); //заменить все эти три функции на одну - возвращающую вектор соседей
    //специфические действия вынести в функции алгоритмов
    void addNeighborsBestFirst(Cell* cell, std::priority_queue<Cell*, QVector<Cell*>, comparatorBestFirst> &q);
    bool bfs(); //pathfinding algorithms
    bool bestFirst();
    bool aStar()
    {

    }

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

public slots:
    void setBfsAlgorithm();
    void setBestFirstAlgorithm();
    void setAStarAlgorithm()
    {
        _algorithm = &Model::aStar;
        qDebug() << "A*";
    }
};

#endif // MODEL_H
