#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QColor>

using Gridtype = QVector<QVector<int>>;

class Model : QObject
{
    Q_OBJECT

    Gridtype _grid;
    bool _gameOn = false;
    size_t _col;
    size_t _row;

public:
    Model(size_t col = 10, size_t row = 10, QObject *parent = nullptr);

    Gridtype& getGrid();

    void clear();

    void addFigure(int num);

    void moveTo(size_t oldRow, size_t oldCol, size_t newRow, size_t newCol)
    {
        if ( (_grid[oldRow][oldCol] != 0) and (_grid[newRow][newCol] == 0) )
        {
            std::swap(_grid[oldRow][oldCol], _grid[newRow][newCol]);
        }
    }

};

#endif // MODEL_H
