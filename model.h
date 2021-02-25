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

struct compare{
    bool operator()(const Cell* l, const Cell* r)
    {
        return l->heuristic() > r->heuristic();
    };
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
    void addNeighborsBFS(Cell* cell, QQueue<Cell*> &q);
    void addNeighborsBestFirst(Cell* cell, std::priority_queue<Cell*, QVector<Cell*>, compare> &q)
    {
        //add left
        int x = cell->x();
        int y = cell->y();
        if ( (((x - 1) >= 0) && !_grid[y][x-1].visited() &&
              _grid[y][x - 1].cellType() == FigureType::EMPTY) ||
             ((y == _fromRow) && ((x-1) == _fromCol)))
        {
            Cell* left = &_grid[y][x - 1];
            left->setParent(&_grid[y][x]);
            left->setVisited(true);
            int heuristic = abs(left->x() - _fromCol) + abs(left->y() - _fromRow);
            left->setHeuristic(heuristic);
            q.push(left);
        }
        //add right
        if( (((x + 1) < _col) && !_grid[y][x + 1].visited() &&
             _grid[y][x + 1].cellType() == FigureType::EMPTY) ||
                ((y == _fromRow) && ((x+1) == _fromCol)))
        {
            Cell* right = &_grid[y][x + 1];
            right->setParent(&_grid[y][x]);
            right->setVisited(true);
            int heuristic = abs(right->x() - _fromCol) + abs(right->y() - _fromRow);
            right->setHeuristic(heuristic);
            q.push(right);
        }
        //add up
        if ( (((y - 1) >= 0) && !_grid[y -1][x].visited() &&
              _grid[y - 1][x].cellType() == FigureType::EMPTY) ||
             (((y-1) == _fromRow) && (x == _fromCol)))
        {
            Cell* up = &_grid[y - 1][x];
            up->setParent(&_grid[y][x]);
            up->setVisited(true);
            int heuristic = abs(up->x() - _fromCol) + abs(up->y() - _fromRow);
            up->setHeuristic(heuristic);
            q.push(up);
        }
        //add down
        if ( (((y + 1) < _row) && !_grid[y + 1][x].visited() &&
              _grid[y + 1][x].cellType() == FigureType::EMPTY) ||
             (((y+1) == _fromRow) && (x == _fromCol)))
        {
            Cell* down = &_grid[y + 1][x];
            down->setVisited(true);
            int heuristic = abs(down->x() - _fromCol) + abs(down->y() - _fromRow);
            down->setHeuristic(heuristic);
            down->setParent(&_grid[y][x]);
            q.push(down);
        }
    }

    bool bfs(); //pathfinding algorithm
    bool bestFirst() //pathfinding algorithm
    {
        clearVisited();


        std::priority_queue <Cell*, QVector<Cell*>, compare> pq;
        Cell *start = &_grid[_toRow][_toCol];
        int heuristic = abs(_fromCol - start->x()) + abs(_fromRow - start->y());
        start->setHeuristic(heuristic);
        pq.push(&_grid[_toRow][_toCol]);
        _grid[_toRow][_toCol].setVisited(true);
        while(!pq.empty())
        {
            Cell* cell = pq.top();
            pq.pop();
            if (cell == &_grid[_fromRow][_fromCol])
            {
                return true;
            }
            addNeighborsBestFirst(cell, pq);
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
    void setBfsAlgorithm()
    {
        _algorithm = &Model::bfs;
        qDebug() << "setup bfs";
    }
    void setBestFirstAlgorithm()
    {
        _algorithm = &Model::bestFirst;
        qDebug() << "setup bestFirst";
    }
};

#endif // MODEL_H
