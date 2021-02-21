#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QColor>
#include <QDebug>
#include <QSet>

using Gridtype = QVector<QVector<int>>;
using cell = int;

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

    void horizontalCheck(QSet<cell*> &set, int row, int col);
    void verticalCheck(QSet<cell*> &set, int row, int col);
    void rightDiagonalCheck(QSet<cell*> &set, int row, int col);
    void leftDiagonalCheck(QSet<cell*> &set, int row, int col);
    int deleteSet(QSet<cell*> &set)
    {
        for (auto &point : set)
        {
            *point = 0;
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

    void addFigures(int row, int col, int figureType);
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
    void doStep()
    {
        //pathFinding();
        if (moveTo(_fromRow, _fromCol, _toRow, _toCol))
        {
            if (!checkAndDeleteLines(_toRow, _toCol))
            {
                //addRandomFigures(3);
            }
        }
    }

signals:
    void scoreChanged(int newScore);
};

#endif // MODEL_H
