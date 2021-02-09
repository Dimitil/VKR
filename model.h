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

    void addRandomFigures(int num);

    void moveTo(size_t oldRow, size_t oldCol, size_t newRow, size_t newCol);

    void addFigures(size_t row, size_t col, int figureType);

    bool checkLines()
    {

    }

};

#endif // MODEL_H
