#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QColor>
#include <QDebug>

using Gridtype = QVector<QVector<int>>;

struct needToDeleteCells{
    int down;
    int right;
    int downLeft;
    int downRight;
};

class Model : QObject
{
    Q_OBJECT

    Gridtype _grid;
    bool _gameOn = false;
    int _col;
    int _row;
    int _score = 0;
public:
    Model(int col = 10, int row = 10, QObject *parent = nullptr);

    Gridtype& getGrid();

    void clear();

    void addRandomFigures(int num);

    void moveTo(int oldRow, int oldCol, int newRow, int newCol);

    void addFigures(int row, int col, int figureType);

    void checkAndDeleteLines(int equalCount)
    {
        QVector<QVector<needToDeleteCells>> toDelete(_row);
        for(int i = 0 ; i < toDelete.size(); i++)
        {
            toDelete[i].resize(_col);
        }

        for(int r = 0; r < _row; r++)
        {
            for(int c = 0; c < _col; c++)
            {
                if (_grid[r][c] == 0)
                {
                    continue;
                }
                toDelete[r][c].right = rightCheck(r, c);
                toDelete[r][c].down = downCheck(r, c);
                toDelete[r][c].downRight =  downRightCheck(r, c);
                toDelete[r][c].downLeft = downLeftCheck(r, c);
            }
        }

        for(int r = 0; r < _row; r++)
        {
            for(int c = 0; c < _col; c++)
            {
                deleteRight(r,c,equalCount, toDelete[r][c].right);
                deleteDown(r,c,equalCount, toDelete[r][c].down);
                deleteDownRight(r,c,equalCount, toDelete[r][c].downRight);
                deleteDownLeft(r,c,equalCount, toDelete[r][c].downLeft);
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

    void deleteRight(int row, int col, int equalCount, int equalRight)
    {
        if (equalRight >= equalCount-1)
        {
            for ( int i = 0; i <= equalRight; i++)
            {
                _grid[row][col+i] = 0;
            }
        }
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

    void deleteDown(int row, int col, int equalCount, int equalDown)
    {
        if (equalDown >= equalCount-1)
        {
            for ( int i = 0; i <= equalDown; i++)
            {
                _grid[row+i][col] = 0;
            }
        }
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

    void deleteDownRight(int row, int col, int equalCount, int equalDownRight)
    {
        if (equalDownRight >= equalCount-1)
        {
            for ( int i = 0; i <= equalDownRight; i++)
            {
                _grid[row+i][col+i] = 0;
            }
        }
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

    void deleteDownLeft(int row, int col, int equalCount, int equalDownLeft)
    {
        if (equalDownLeft >= equalCount-1)
        {
            for ( int i = 0; i <= equalDownLeft; i++)
            {
                _grid[row+i][col-i] = 0;
            }
        }
    }

};

#endif // MODEL_H
