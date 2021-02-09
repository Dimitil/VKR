#include "model.h"
#include <QDebug>


Model::Model(size_t col, size_t row, QObject *parent) : _col(col),
    _row(row), QObject(parent)
{
    for(size_t i = 0; i < row; i++) {
        _grid.push_back(QVector<int>(col, 0));
    }
}

Gridtype &Model::getGrid() { return _grid;}

void Model::clear()
{
    for (size_t r = 0; r < _row; r++)
    {
        for(size_t c = 0; c < _col; c++)
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

        size_t r = 0;
        size_t c = 0;
        do {
            r = rand()%_row;
            c = rand()%_col;
        }
        while( _grid[r][c] != 0);
        _grid[r][c] = f;
    }
}

void Model::moveTo(size_t oldRow, size_t oldCol, size_t newRow, size_t newCol)  //NEED TEST
{
    if ( (_grid[oldRow][oldCol] != 0) and (_grid[newRow][newCol] == 0) )
    {
        std::swap(_grid[oldRow][oldCol], _grid[newRow][newCol]);
    }
}

void Model::addFigures(size_t row, size_t col, int figureType)
{
    _grid[row][col] = figureType;
}
