#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QColor>
#include <QDebug>
#include <QSet>
#include <QQueue>

enum class FigureType
{
    EMPTY,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE
};


class Cell{
    FigureType _type;
    Cell* _parent = nullptr;
    bool _visited = false;
    int _y;
    int _x;

public:
    int y() const
    {
        return _y;
    }
    int x() const
    {
        return _x;
    }
    void setY(int y)
    {
        _y=y;
    }
    void setX(int x)
    {
        _x=x;
    }

    bool visited() const
    {
        return _visited;
    }
    void setVisited(bool b)
    {
        _visited = b;
    }
    void setType(FigureType type)
    {
        _type = type;
    }
    void setParent(Cell* newParent)
    {
        _parent = newParent;
    }
    Cell* cellParent() const
    {
        return _parent;
    }
    Cell(FigureType type) : _type(type) {}
    FigureType cellType() const
    {
        return _type;
    }
    Cell() : _type(FigureType::EMPTY), _parent(nullptr){}
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
    bool userMadeMove = false;

    void horizontalCheck(QSet<Cell*> &set, int row, int col);
    void verticalCheck(QSet<Cell*> &set, int row, int col);
    void rightDiagonalCheck(QSet<Cell*> &set, int row, int col);
    void leftDiagonalCheck(QSet<Cell*> &set, int row, int col);
    int deleteSet(QSet<Cell*> &set)
    {
        for (auto &cell : set)
        {
            cell->setType(FigureType::EMPTY);
        }
        return set.size();
    }
    void addScore(int score);
    bool moveTo(int oldRow, int oldCol, int newRow, int newCol);
    void clear();
public:

    Model(int col = 10, int row = 10, QObject *parent = nullptr);

    Gridtype& getGrid();
    void addRandomFigures(int num);

    void addFigures(int row, int col, FigureType figureType);
    int checkAndDeleteLines(int row, int col);
    void resize(int row, int col);
    void setFrom(int row, int col)
    {
        _fromCol = col;
        _fromRow = row;
    }
    void setTo(int row, int col)
    {
        _toCol = col;
        _toRow = row;
    }
    void coordinateAllCells()
    {
        for(int r = 0; r < _row; r++)
        {
            for(int c = 0; c < _col; c++)
            {
                _grid[r][c].setY(r);
                _grid[r][c].setX(c);
            }
        }
    }

    void doStep()
    {
        if(bfs())
        {
            if (moveTo(_fromRow, _fromCol, _toRow, _toCol))
            {
                if (!checkAndDeleteLines(_toRow, _toCol))
                {
                    addRandomFigures(3);
                }
            }
        }
    }

    void clearVisited()
    {
        for (int r = 0; r < _row; r++)
        {
            for(int c = 0; c < _col; c++)
            {
                _grid[r][c].setVisited(false);
            }
        }
    }

    void addNeighbors(Cell* cell, QQueue<Cell*> &q)
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
            q.push_back(left);
        }
        //add right
        if( (((x + 1) < _col) && !_grid[y][x + 1].visited() &&
                _grid[y][x + 1].cellType() == FigureType::EMPTY) ||
                ((y == _fromRow) && ((x+1) == _fromCol)))
        {
            Cell* right = &_grid[y][x + 1];
            right->setParent(&_grid[y][x]);
            right->setVisited(true);
            q.push_back(right);
        }
        //add up
        if ( (((y - 1) >= 0) && !_grid[y -1][x].visited() &&
             _grid[y - 1][x].cellType() == FigureType::EMPTY) ||
            (((y-1) == _fromRow) && (x == _fromCol)))
        {
            Cell* up = &_grid[y - 1][x];
            up->setParent(&_grid[y][x]);
            up->setVisited(true);
            q.push_back(up);
        }
        //add down
        if ( (((y + 1) < _row) && !_grid[y + 1][x].visited() &&
             _grid[y + 1][x].cellType() == FigureType::EMPTY) ||
           (((y+1) == _fromRow) && (x == _fromCol)))
        {
            Cell* down = &_grid[y + 1][x];
            down->setVisited(true);
            down->setParent(&_grid[y][x]);
            q.push_back(down);
        }
    }

    bool bfs()
    {
        clearVisited();
        coordinateAllCells(); //debug - нужно оптимизировать установку x и y при добавлении фигуры
        QQueue <Cell*> q;
        q.push_back(&_grid[_toRow][_toCol]);
        _grid[_toRow][_toCol].setVisited(true);
        while(!q.empty())
        {
            Cell* cell = q.back();
            q.pop_back();
            if (cell == &_grid[_fromRow][_fromCol])
            {
                return true;
            }
            addNeighbors(cell, q);
        }
        return false;
    }

signals:
    void scoreChanged(int newScore);
};

#endif // MODEL_H
