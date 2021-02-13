#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QColor>
#include <QDebug>

using Gridtype = QVector<QVector<int>>;

struct toDeleteCells{
    int x;
    int y;
    int count;
};

class Model : QObject
{
    Q_OBJECT

    Gridtype _grid;
    bool _gameOn = false;
    int _col;
    int _row;
    int _score = 0;

    toDeleteCells horizontalCheck(int row, int col);
    toDeleteCells verticalCheck(int row, int col);
    toDeleteCells rightDiagonalCheck(int row, int col);
    toDeleteCells leftDiagonalCheck(int row, int col);
    int deleteRight(toDeleteCells point, int equalCount);
    int deleteDown(toDeleteCells point, int equalCount);
    int deleteDownRight(toDeleteCells point, int equalCount);
    int deleteDownLeft(toDeleteCells point, int equalCount);
    void addScore(int score);
    void clear();
public:

    Model(int col = 10, int row = 10, QObject *parent = nullptr);

    Gridtype& getGrid();
    void addRandomFigures(int num);
    void moveTo(int oldRow, int oldCol, int newRow, int newCol);
    void addFigures(int row, int col, int figureType);
    int checkAndDeleteLines(int equalCount, int row, int col);
    void resize(int row, int col);
};

#endif // MODEL_H
