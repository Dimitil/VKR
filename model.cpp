#include "model.h"
#include <QDebug>


Model::Model(int col, int row, QObject *parent) : QObject(parent),
    _col(col), _row(row)
{
    for(int i = 0; i < row; i++) {
        _grid.push_back(QVector<int>(col, 0));
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
    }
}

void Model::moveTo(int oldRow, int oldCol, int newRow, int newCol)
{
    if ( (_grid[oldRow][oldCol] != 0) and (_grid[newRow][newCol] == 0) )
    {
        std::swap(_grid[oldRow][oldCol], _grid[newRow][newCol]);
    }
}

void Model::addFigures(int row, int col, int figureType)
{
    _grid[row][col] = figureType;
}
