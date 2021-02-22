#include "model.h"
#include <QDebug>


Model::Model(int col, int row, QObject *parent) : QObject(parent),
    _col(col), _row(row)
{
    for(int i = 0; i < row; i++)
    {
        _grid.push_back(QVector<Cell>(col, FigureType::EMPTY));
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
        _grid[r].fill(FigureType::EMPTY);
    }
}

Gridtype &Model::getGrid() { return _grid;}

void Model::clear()
{
    for (int r = 0; r < _row; r++)
    {
        for(int c = 0; c < _col; c++)
        {
            _grid[r][c].setType(FigureType::EMPTY);
        }
    }
}

void Model::addRandomFigures(int num)
{
    for (int i = 0 ; i < num; i++)
    {
        int t = rand() % 4 + 1;

        int r = 0;
        int c = 0;
        do {
            r = rand() % _row;
            c = rand() % _col;
        }
        while( _grid[r][c].cellType() != FigureType::EMPTY );
        FigureType newType = static_cast<FigureType>(t);
        _grid[r][c].setType(newType);
        if (checkAndDeleteLines(r, c))
        {
            --i;
        }
    }
}

bool Model::moveTo(int oldRow, int oldCol, int newRow, int newCol)
{
    if ( (_grid[oldRow][oldCol].cellType() != FigureType::EMPTY) and
         (_grid[newRow][newCol].cellType() == FigureType::EMPTY) )
    {
        //std::swap(_grid[oldRow][oldCol], _grid[newRow][newCol]);
        FigureType ft = _grid[oldRow][oldCol].cellType();
        _grid[oldRow][oldCol].setType(FigureType::EMPTY);
        _grid[newRow][newCol].setType(ft);
        return true;
    }
    return false;
}

void Model::addFigures(int row, int col, FigureType figureType)
{
    _grid[row][col].setType(figureType);
}

int Model::checkAndDeleteLines(int row, int col)
{
    QSet<Cell*> set;

    horizontalCheck(set, row, col);
    verticalCheck(set, row, col);
    rightDiagonalCheck(set, row, col);
    leftDiagonalCheck(set, row, col);

    int totalDeleted = deleteSet(set);
    addScore(totalDeleted);
    return totalDeleted;
}

void Model::horizontalCheck(QSet<Cell*> &set, int row, int col)
{
    //left check;
    int count = 0;
    for ( int c = col - 1; c >= 0; c--)
    {
        if (_grid[row][col].cellType() == _grid[row][c].cellType())
        {
            count++;
        }
        else {
            break;
        }
    }
    int left = col - count;
    //right check;
    for( int c = col + 1; c < _col; c++)
    {
        if (_grid[row][col].cellType() == _grid[row][c].cellType())
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

void Model::verticalCheck(QSet<Cell*> &set, int row, int col)
{
    int count = 0;
    //up check
    for (int r = row - 1; r >= 0; r--)
    {
        if(_grid[row][col].cellType() == _grid[r][col].cellType())
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
        if(_grid[row][col].cellType() == _grid[r][col].cellType())
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

void Model::rightDiagonalCheck(QSet<Cell*> &set, int row, int col)
{
    int count = 0;
    //up check
    for (int r = row - 1, c = col - 1; (c >= 0) && (r >= 0) ; --r, --c)
    {
        if (_grid[row][col].cellType() == _grid[r][c].cellType())
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
        if (_grid[row][col].cellType() == _grid[r][c].cellType())
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

void Model::leftDiagonalCheck(QSet<Cell*> &set, int row, int col)
{
    int count = 0;
    //up check
    for (int r = row - 1, c = col + 1; (c < _col) && (r >= 0) ; --r, ++c)
    {
        if (_grid[row][col].cellType() == _grid[r][c].cellType())
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
        if (_grid[row][col].cellType() == _grid[r][c].cellType())
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
