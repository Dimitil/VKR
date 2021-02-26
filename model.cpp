#include "model.h"
#include <QDebug>


Model::Model(int col, int row, QObject *parent) : QObject(parent),
    _col(col), _row(row)
{
    for(int r = 0; r < row; r++)
    {
        _grid.push_back(QVector<Cell>(col, FigureType::EMPTY));
    }
    coordinateAllCells();
    setBfsAlgorithm();  //debug()
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
    coordinateAllCells();
}

int Model::fromRow() const
{
    return _fromRow;
}

int Model::fromCol() const
{
    return _fromCol;
}

int Model::toRow() const
{
    return _toRow;
}

int Model::toCol() const
{
    return _toCol;
}

int Model::row() const
{
    return _row;
}

int Model::col() const
{
    return _col;
}

void Model::setFrom(int row, int col)
{
    _fromCol = col;
    _fromRow = row;
}

void Model::setTo(int row, int col)
{
    _toCol = col;
    _toRow = row;
}

void Model::coordinateAllCells()
{
    for(int r = 0; r < _row; r++)
    {
        for(int c = 0; c < _col; c++)
        {
            _grid[r][c].setY(r);
            _grid[r][c].setX(c);
        }
    }
}

bool Model::doStep()
{
    if((this->*_algorithm)())
    {
        if (moveTo(_fromRow, _fromCol, _toRow, _toCol))
        {
            if (!checkAndDeleteLines(_toRow, _toCol))
            {
                //addRandomFigures(3);
            }
            return true;
        }
    }
    return false;
}

void Model::setBfsAlgorithm()
{
    _algorithm = &Model::bfs;
    qDebug() << "setup bfs";
}

void Model::setBestFirstAlgorithm()
{
    _algorithm = &Model::bestFirst;
    qDebug() << "setup bestFirst";
}

void Model::clearVisited()
{
    for (int r = 0; r < _row; r++)
    {
        for(int c = 0; c < _col; c++)
        {
            _grid[r][c].setVisited(false);
        }
    }
}

void Model::addNeighborsBFS(Cell *cell, QQueue<Cell*> &q)
{
    //add left
    int x = cell->x();
    int y = cell->y();

    if ( (((x - 1) >= 0) && !_grid[y][x-1].visited() &&
          _grid[y][x - 1].cellType() == FigureType::EMPTY) ||
         ((y == _fromRow) && ((x-1) == _fromCol)))
    {
        Cell* left = &_grid[y][x - 1];
        left->setParent(&_grid[y][x]);
        left->setVisited(true);
        q.push_back(left);
    }
    //add right
    if( (((x + 1) < _col) && !_grid[y][x + 1].visited() &&
         _grid[y][x + 1].cellType() == FigureType::EMPTY) ||
            ((y == _fromRow) && ((x+1) == _fromCol)))
    {
        Cell* right = &_grid[y][x + 1];
        right->setParent(&_grid[y][x]);
        right->setVisited(true);
        q.push_back(right);
    }
    //add up
    if ( (((y - 1) >= 0) && !_grid[y -1][x].visited() &&
          _grid[y - 1][x].cellType() == FigureType::EMPTY) ||
         (((y-1) == _fromRow) && (x == _fromCol)))
    {
        Cell* up = &_grid[y - 1][x];
        up->setParent(&_grid[y][x]);
        up->setVisited(true);
        q.push_back(up);
    }
    //add down
    if ( (((y + 1) < _row) && !_grid[y + 1][x].visited() &&
          _grid[y + 1][x].cellType() == FigureType::EMPTY) ||
         (((y+1) == _fromRow) && (x == _fromCol)))
    {
        Cell* down = &_grid[y + 1][x];
        down->setVisited(true);
        down->setParent(&_grid[y][x]);
        q.push_back(down);
    }
}

void Model::addNeighborsBestFirst(Cell *cell, std::priority_queue<Cell *, QVector<Cell *>, comparatorBestFirst> &q)
{
    //add left
    int x = cell->x();
    int y = cell->y();
    if ( (((x - 1) >= 0) && !_grid[y][x-1].visited() &&
          _grid[y][x - 1].cellType() == FigureType::EMPTY) ||
         ((y == _fromRow) && ((x-1) == _fromCol)))
    {
        Cell* left = &_grid[y][x - 1];
        left->setParent(&_grid[y][x]);
        left->setVisited(true);
        int heuristic = abs(left->x() - _fromCol) + abs(left->y() - _fromRow);
        left->setHeuristic(heuristic);
        q.push(left);
    }
    //add right
    if( (((x + 1) < _col) && !_grid[y][x + 1].visited() &&
         _grid[y][x + 1].cellType() == FigureType::EMPTY) ||
            ((y == _fromRow) && ((x+1) == _fromCol)))
    {
        Cell* right = &_grid[y][x + 1];
        right->setParent(&_grid[y][x]);
        right->setVisited(true);
        int heuristic = abs(right->x() - _fromCol) + abs(right->y() - _fromRow);
        right->setHeuristic(heuristic);
        q.push(right);
    }
    //add up
    if ( (((y - 1) >= 0) && !_grid[y -1][x].visited() &&
          _grid[y - 1][x].cellType() == FigureType::EMPTY) ||
         (((y-1) == _fromRow) && (x == _fromCol)))
    {
        Cell* up = &_grid[y - 1][x];
        up->setParent(&_grid[y][x]);
        up->setVisited(true);
        int heuristic = abs(up->x() - _fromCol) + abs(up->y() - _fromRow);
        up->setHeuristic(heuristic);
        q.push(up);
    }
    //add down
    if ( (((y + 1) < _row) && !_grid[y + 1][x].visited() &&
          _grid[y + 1][x].cellType() == FigureType::EMPTY) ||
         (((y+1) == _fromRow) && (x == _fromCol)))
    {
        Cell* down = &_grid[y + 1][x];
        down->setVisited(true);
        int heuristic = abs(down->x() - _fromCol) + abs(down->y() - _fromRow);
        down->setHeuristic(heuristic);
        down->setParent(&_grid[y][x]);
        q.push(down);
    }
}

bool Model::bfs()
{
    clearVisited();
    QQueue <Cell*> q;
    q.push_back(&_grid[_toRow][_toCol]);
    _grid[_toRow][_toCol].setVisited(true);
    while(!q.empty())
    {
        Cell* cell = q.back();
        q.pop_back();
        if (cell == &_grid[_fromRow][_fromCol])
        {
            return true;
        }
        addNeighborsBFS(cell, q);
    }
    return false;
}

bool Model::bestFirst() //pathfinding algorithm
{
    clearVisited();
    std::priority_queue <Cell*, QVector<Cell*>, comparatorBestFirst> pq;
    Cell *start = &_grid[_toRow][_toCol];
    int heuristic = abs(_fromCol - start->x()) + abs(_fromRow - start->y());
    start->setHeuristic(heuristic);
    pq.push(&_grid[_toRow][_toCol]);
    _grid[_toRow][_toCol].setVisited(true);
    while(!pq.empty())
    {
        Cell* cell = pq.top();
        pq.pop();
        if (cell == &_grid[_fromRow][_fromCol])
        {
            return true;
        }
        addNeighborsBestFirst(cell, pq);
    }
    return false;
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

int Model::deleteSet(QSet<Cell *> &set)
{
    for (auto &cell : set)
    {
        cell->setType(FigureType::EMPTY);
    }
    return set.size();
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
