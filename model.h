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
    void clearMoveCost();
    void clearParent()
    {
        for (int r = 0; r < _row; r++)
        {
            for(int c = 0; c < _col; c++)
            {
                _grid[r][c].setParent(nullptr);
            }
        }
    }
    QVector<Cell*> neighbors(Cell* cell);

    bool bfs(); //pathfinding algorithms
    bool bestFirst();
    bool aStar()
    {
        clearMoveCost();

        std::priority_queue <Cell*, QVector<Cell*>, comparatorAStar> pq;
        Cell *start = &_grid[_toRow][_toCol];
        Cell * goal = &_grid[_fromRow][_fromCol];
        int heuristic = abs(_fromCol - start->x()) + abs(_fromRow - start->y());
        int moveCost = 1;
        start->setHeuristic(heuristic);
        start->setMoveCost(moveCost);
        pq.push(start);

        while(!pq.empty())
        {
            Cell* cell = pq.top();
            pq.pop();
            if (cell == goal)
            {
                return true;
            }

            for (auto& next : neighbors(cell))
            {
                moveCost ++;
                if( !next->moveCost() || ( next->moveCost() > moveCost) )
                {
                    int heuristic = abs(next->x() - _fromCol) + abs(next->y() - _fromRow);
                    next->setParent(cell);
                    next->setHeuristic(heuristic);
                    next->setMoveCost(moveCost);
                    pq.push(next);
                }
            }
        }
        return false;
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
    void setAStarAlgorithm();
};

#endif // MODEL_H
