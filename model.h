#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QColor>
#include <QDebug>

using Gridtype = QVector<QVector<int>>;

class Model : QObject
{
    Q_OBJECT

    Gridtype _grid;
    bool _gameOn = false;
    int _col;
    int _row;

public:
    Model(int col = 10, int row = 10, QObject *parent = nullptr);

    Gridtype& getGrid();

    void clear();

    void addRandomFigures(int num);

    void moveTo(int oldRow, int oldCol, int newRow, int newCol);

    void addFigures(int row, int col, int figureType);

    void checkLines(int equalCount)
    {
        for(int r = 0; r < _row; r++)
        {
            for(int c = 0; c < _col; c++)
            {
                if (_grid[r][c] == 0)
                {
                    continue;
                }

                size_t rightEq = rightCheck(r, c);

                size_t downEq = downCheck(r, c);

                size_t downRightEq =  downRightCheck(r, c);

                size_t downLeftEq = downLeftCheck(r, c);   //завести какой нибудь контейнер и в него складывать
                //qDebug() << r << c << downLeftEq;          //все что подлежит удалению
            }
        }
    }

    int rightCheck(int row, int col)
    {
        int equalNumToRight = 0;
        for (int i = 1; i + col < _col; i++)
        {
            if(_grid[row][col] == _grid[row][col+i])
            {
                equalNumToRight++;
            }
            else {
                break;
            }
        }
        return equalNumToRight;
    }

    int downCheck(int row, int col)
    {
        int equalNumToDown = 0;
        for (int i = 1; i + row < _row; i++)
        {
            if(_grid[row][col] == _grid[row + i][col])
            {
                equalNumToDown++;
            }
            else {
                break;
            }
        }
        return equalNumToDown;
    }

    int downRightCheck(int row, int col)
    {
        int equalNumToDownRight = 0;
        for (int r = 1, c = 1; (c + col < _col) && (r + row < _row) ; r++, c++)
        {
            if(_grid[row][col] == _grid[row+r][col+c])
            {
                equalNumToDownRight++;
            }
            else {
                break;
            }
        }
        return equalNumToDownRight;
    }

    int downLeftCheck(int row, int col)
    {
        int equalNumToDownLeft = 0;
        for (int r = 1, c = 1; ( col - c >= 0 ) && (r + row < _row) ; r++, c++)
        {
            if(_grid[row][col] == _grid[row+r][col-c])
            {
                equalNumToDownLeft++;
            }
            else {
                break;
            }
        }
        return equalNumToDownLeft;
    }
};

#endif // MODEL_H
