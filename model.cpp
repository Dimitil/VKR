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
    QSet<cell*> set;

    horizontalCheck(set, row, col);
    verticalCheck(set, row, col);
    rightDiagonalCheck(set, row, col);
    leftDiagonalCheck(set, row, col);

    int totalDeleted = deleteSet(set);
    addScore(totalDeleted);
    return totalDeleted;
}

void Model::horizontalCheck(QSet<cell*> &set, int row, int col)
{
    //left check;
    int count = 0;
    for ( int c = col - 1; c >= 0; c--)
    {
        if (_grid[row][col] == _grid[row][c])
        {
            count++;
        }
        else {
            break;
        }
    }
    cell left = col - count;
    //right check;
    for( int c = col + 1; c < _col; c++)
    {
        if (_grid[row][col] == _grid[row][c])
        {
            count++;
        }
        else {
            break;
        }
    }
    //filling set
    if (count >= _equalCount - 1)
    {
        for (int i = 0; i<=count; i++)
        {
            set.insert(&_grid[row][left+i]);
        }
    }
}

void Model::verticalCheck(QSet<cell*> &set, int row, int col)
{
    int count = 0;
    //up check
    for (int r = row - 1; r >= 0; r--)
    {
        if(_grid[row][col] == _grid[r][col])
        {
            count++;
        }
        else {
            break;
        }
    }
    int up = row - count;
    //down check
    for ( int r = row +1 ; r < _row; r++)
    {
        if(_grid[row][col] == _grid[r][col])
        {
            count++;
        }
        else {
            break;
        }
    }
    //filling set
    if (count >= _equalCount - 1)
    {
        for (int i = 0; i<=count; i++)
        {
            set.insert(&_grid[up + i][col]);
        }
    }
}

void Model::rightDiagonalCheck(QSet<cell*> &set, int row, int col)
{
    int count = 0;
    //up check
    for (int r = row - 1, c = col - 1; (c >= 0) && (r >= 0) ; --r, --c)
    {
        if (_grid[row][col] == _grid[r][c])
        {
            count ++;
        }
        else {
            break;
        }
    }
    int up = row - count;
    int left = col - count;
    //down check
    for (int r = row + 1, c = col + 1; (c < _col) && (r < _row) ; ++r, ++c)
    {
        if (_grid[row][col] == _grid[r][c])
        {
            count++;
        }
        else {
            break;
        }
    }
    if (count >= _equalCount - 1)
    {
        for (int i = 0; i<=count; i++)
        {
            set.insert(&_grid[up + i][left+i]);
        }
    }
}

void Model::leftDiagonalCheck(QSet<cell*> &set, int row, int col)
{
    int count = 0;
    //up check
    for (int r = row - 1, c = col + 1; (c < _col) && (r >= 0) ; --r, ++c)
    {
        if (_grid[row][col] == _grid[r][c])
        {
            count++;
        }
        else {
            break;
        }
    }
    int up = row - count;
    int right = col + count;
    //down check
    for (int r = row + 1, c = col - 1; (c >= 0) && (r < _row) ; ++r, --c)
    {
        if (_grid[row][col] == _grid[r][c])
        {
            count ++;
        }
        else {
            break;
        }
    }
    if (count >= _equalCount - 1)
    {
        for (int i = 0; i<=count; i++)
        {
            set.insert(&_grid[up + i][right - i]);
        }
    }
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
