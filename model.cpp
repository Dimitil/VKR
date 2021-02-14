#include "model.h"
#include <QDebug>


Model::Model(int col, int row, QObject *parent) : QObject(parent),
    _col(col), _row(row)
{
    for(int i = 0; i < row; i++) {
        _grid.push_back(QVector<int>(col, 0));
    }
}

void Model::resize(int row, int col)
{
    clear();
    _row = row;
    _col = col;
    _grid.resize(row);
    for(int r = 0; r< _row; r++)
    {
        _grid[r].resize(col);
        _grid[r].fill(0);
    }
}

Gridtype &Model::getGrid() { return _grid;}

void Model::clear()
{
    for (int r = 0; r < _row; r++)
    {
        for(int c = 0; c < _col; c++)
        {
            _grid[r][c] = 0;
        }
    }
}

void Model::addRandomFigures(int num)
{
    for (int i = 0 ; i < num; i++)
    {
        int f = rand() % 4 + 1;

        int r = 0;
        int c = 0;
        do {
            r = rand()%_row;
            c = rand()%_col;
        }
        while( _grid[r][c] != 0);
        _grid[r][c] = f;
        if (checkAndDeleteLines(r, c))
        {
            --i;
        }
    }
}

bool Model::moveTo(int oldRow, int oldCol, int newRow, int newCol)
{
    if ( (_grid[oldRow][oldCol] != 0) and (_grid[newRow][newCol] == 0) )
    {
        std::swap(_grid[oldRow][oldCol], _grid[newRow][newCol]);
        return true;
    }
    return false;
}

void Model::addFigures(int row, int col, int figureType)
{
    _grid[row][col] = figureType;
}

int Model::checkAndDeleteLines(int row, int col)
{
    toDeleteCells deleteThisRight     = horizontalCheck(row, col);
    toDeleteCells deleteThisDown      = verticalCheck(row, col);
    toDeleteCells deleteThisDownRight = rightDiagonalCheck(row, col);
    toDeleteCells deleteThisDownLeft  = leftDiagonalCheck(row, col);

    int rightDeleted     = deleteRight(deleteThisRight);
    int downDeleted      = deleteDown(deleteThisDown);
    int downRightDeleted = deleteDownRight(deleteThisDownRight);
    int downLeftDeleted  = deleteDownLeft(deleteThisDownLeft);

    int totalDeleted = rightDeleted + downDeleted + downRightDeleted + downLeftDeleted;
    addScore(totalDeleted);
    return totalDeleted;
}

toDeleteCells Model::horizontalCheck(int row, int col)
{
    toDeleteCells horizontal;
    horizontal.y = row;
    int equal = 0;
    //left check;
    for ( int c = col - 1; c >= 0; c--)
    {
        if (_grid[row][col] == _grid[row][c])
        {
            equal++;
        }
        else {
            break;
        }
    }
    horizontal.x = col - equal;
    //right check;
    for( int c = col + 1; c < _col; c++)
    {
        if (_grid[row][col] == _grid[row][c])
        {
            equal++;
        }
        else {
            break;
        }
    }

    horizontal.count = equal;
    return horizontal;
}

toDeleteCells Model::verticalCheck(int row, int col)
{
    toDeleteCells vertical;
    vertical.x = col;
    int equal = 0;
    //up check
    for (int r = row - 1; r >= 0; r--)
    {
        if(_grid[row][col] == _grid[r][col])
        {
            equal++;
        }
        else {
            break;
        }
    }
    vertical.y = row - equal;
    //down check
    for ( int r = row +1 ; r < _row; r++)
    {
        if(_grid[row][col] == _grid[r][col])
        {
            equal++;
        }
        else {
            break;
        }
    }
    vertical.count = equal;
    return vertical;
}

toDeleteCells Model::rightDiagonalCheck(int row, int col)
{
    toDeleteCells rightDiagonal;
    int equal = 0;
    //up check
    for (int r = row - 1, c = col - 1; (c >= 0) && (r >= 0) ; --r, --c)
    {
        if (_grid[row][col] == _grid[r][c])
        {
            equal ++;
        }
        else {
            break;
        }
    }
    rightDiagonal.y = row - equal;
    rightDiagonal.x = col - equal;
    //down check
    for (int r = row + 1, c = col + 1; (c < _col) && (r < _row) ; ++r, ++c)
    {
        if (_grid[row][col] == _grid[r][c])
        {
            equal ++;
        }
        else {
            break;
        }
    }
    rightDiagonal.count = equal;
    return rightDiagonal;
}

toDeleteCells Model::leftDiagonalCheck(int row, int col)
{
    toDeleteCells leftDiagonal;
    int equal = 0;
    //up check
    for (int r = row - 1, c = col + 1; (c < _col) && (r >= 0) ; --r, ++c)
    {
        if (_grid[row][col] == _grid[r][c])
        {
            equal ++;
        }
        else {
            break;
        }
    }
    leftDiagonal.y = row - equal;
    leftDiagonal.x = col + equal;
    //down check
    for (int r = row + 1, c = col - 1; (c >= 0) && (r < _row) ; ++r, --c)
    {
        if (_grid[row][col] == _grid[r][c])
        {
            equal ++;
        }
        else {
            break;
        }
    }
    leftDiagonal.count = equal;
    return leftDiagonal;
}



int Model::deleteRight(toDeleteCells point)
{
    int deleted = 0;
    if (point.count >= _equalCount-1)
    {
        for (int i = 0; i <= point.count; i++)
        {
            if (_grid[point.y][point.x+i] == 0)
            {
                continue;
            }
            _grid[point.y][point.x+i] = 0;
            deleted ++;
        }
    }
    return deleted;
}

int Model::deleteDown(toDeleteCells point)
{
    int deleted = 0;
    if (point.count >= _equalCount-1)
    {
        for ( int i = 0; i <= point.count; i++)
        {
            if (_grid[point.y + i][point.x] == 0)
            {
                continue;
            }
            _grid[point.y + i][point.x] = 0;
            deleted++;
        }
    }
    return deleted;
}

int Model::deleteDownRight(toDeleteCells point)
{
    int deleted = 0;
    if (point.count >= _equalCount - 1)
    {
        for ( int i = 0; i <= point.count; i++)
        {
            if (_grid[point.y + i][point.x + i] == 0)
            {
                continue;
            }
            _grid[point.y + i][point.x + i] = 0;
            deleted++;
        }
    }
    return deleted;
}

int Model::deleteDownLeft(toDeleteCells point)
{
    int deleted = 0;
    if (point.count >= _equalCount - 1)
    {
        for ( int i = 0; i <= point.count; i++)
        {
            if (_grid[point.y + i][point.x - i] == 0)
            {
                continue;
            }
            _grid[point.y + i][point.x - i] = 0;
            deleted++;
        }
    }
    return deleted;
}

void Model::addScore(int deletedCells)
{
    if (deletedCells == _equalCount)
    {
        _score += deletedCells;
    }
    else if (deletedCells > _equalCount)
    {
        int bonus = deletedCells - _equalCount;
        bonus *=2;
        _score += bonus;
        _score += _equalCount;
    }
    emit scoreChanged(_score);
}
