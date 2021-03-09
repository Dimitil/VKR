#include "model.h"
#include <QDebug>
#include <random>
#include <chrono>

Model::Model(QObject *parent) : QObject(parent), _col(0), _row(0)
{
    _mt_rand.seed(time(nullptr));
    _testMode = false;
    setBfsAlgorithm();
    setNormalDifficulty();
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

int Model::score() const
{
    return _score;
}

void Model::testModeOn()
{
    _testMode = true;
}

void Model::testModeOff()
{
    _testMode = false;
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

void Model::setEqualCount(int equalCount)
{
    _equalCount = equalCount;
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
    auto start = std::chrono::high_resolution_clock::now();
    if((this->*_algorithm)())
    {
    auto stop = std::chrono::high_resolution_clock::now();
    long long n = (stop - start).count();
    qDebug() << n;
        if (moveTo(_fromRow, _fromCol, _toRow, _toCol))
        {
            if (!checkAndDeleteLines(_toRow, _toCol) && !_testMode)
            {
                if (!addRandomFigures(3))
                {
                    emit gameOver();
                    return false;
                }
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

void Model::setAStarAlgorithm()
{
    _algorithm = &Model::aStar;
    qDebug() << "setup A*";
}

void Model::setEazyDifficulty()
{
    _difficulty = DifficultyType::EAZY;
    testModeOff();
    resize(4, 4);
    setEqualCount(3);
    addRandomFigures(3);
    setScore(0);
    emit difficultyChanged();
}

void Model::setNormalDifficulty()
{
    _difficulty = DifficultyType::NORMAL;
    testModeOff();
    resize(6, 6);
    setEqualCount(4);
    addRandomFigures(3);
    setScore(0);
    emit difficultyChanged();
}

void Model::setHardDifficulty()
{
    _difficulty = DifficultyType::HARD;
    testModeOff();
    resize(8, 8);
    setEqualCount(5);
    addRandomFigures(3);
    setScore(0);
    emit difficultyChanged();
}

void Model::setExtraHardDifficulty()
{
    _difficulty = DifficultyType::EXTRAHARD;
    testModeOff();
    resize(12, 12);
    setEqualCount(5);
    addRandomFigures(5);
    setScore(0);
    emit difficultyChanged();
}

void Model::testCase1(){
    testModeOn();
    resize(12, 12);
    clear();
    for ( int i = 2; i<10; i++)
    {
        addRandomFigure(2, i);
        addRandomFigure(_row - 3, i);
    }
    for (int i = 2; i < _row - 2; i++)
    {
        addRandomFigure(i, _col - 3);
    }
    addRandomFigure(_row - 3, 1);
    emit difficultyChanged();
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

void Model::clearMoveCost()
{
    for (int r = 0; r < _row; r++)
    {
        for(int c = 0; c < _col; c++)
        {
            _grid[r][c].setMoveCost(0);
        }
    }
}

void Model::clearParent()
{
    for (int r = 0; r < _row; r++)
    {
        for(int c = 0; c < _col; c++)
        {
            _grid[r][c].setParent(nullptr);
        }
    }
}

int Model::maxTypeCount() const
{
    switch (_difficulty)
    {
    case DifficultyType::EAZY:
        return 4;
    case DifficultyType::NORMAL:
        return 5;
    case DifficultyType::HARD:
        return 6;
    case DifficultyType::EXTRAHARD:
        return 6;
    default:
        return 0;
    }
}


FigureType Model::getRandomType()
{
    int t = _mt_rand() % maxTypeCount() + 1;
    FigureType ft = static_cast<FigureType>(t);
    return ft;
}

int Model::getRandomCol()
{
    return _mt_rand() % _col;
}

int Model::getRandomRow()
{
    return _mt_rand() % _col;
}

QVector<Cell *> Model::neighbors(Cell *cell)
{
    QVector <Cell*> neighbors;
    int x = cell->x();
    int y = cell->y();
    //add left
    if ( (((x - 1) >= 0) && _grid[y][x - 1].cellType() == FigureType::EMPTY) ||
         ((y == _fromRow) && ((x-1) == _fromCol)))
    {
        neighbors.push_back(&_grid[y][x - 1]);
    }
    //add right
    if( (((x + 1) < _col) && _grid[y][x + 1].cellType() == FigureType::EMPTY) ||
            ((y == _fromRow) && ((x+1) == _fromCol)))
    {
        neighbors.push_back(&_grid[y][x + 1]);
    }
    //add up
    if ( (((y - 1) >= 0) && _grid[y - 1][x].cellType() == FigureType::EMPTY) ||
         (((y-1) == _fromRow) && (x == _fromCol)))
    {
        neighbors.push_back(&_grid[y - 1][x]);
    }
    //add down
    if ( (((y + 1) < _row) && _grid[y + 1][x].cellType() == FigureType::EMPTY) ||
         (((y+1) == _fromRow) && (x == _fromCol)))
    {
        neighbors.push_back(&_grid[y + 1][x]);
    }
    return neighbors;
}

bool Model::bfs()
{
    clearVisited();
    QQueue <Cell*> q;
    Cell* start = &_grid[_toRow][_toCol];
    Cell* goal = &_grid[_fromRow][_fromCol];
    start->setVisited(true);
    q.push_back(start);
    while(!q.empty())
    {
        Cell* cell = q.front();
        q.pop_front();
        if (cell == goal)
        {
            return true;
        }
        for (auto& next : neighbors(cell) )
        {
            if (!next->visited())
            {
                next->setVisited(true);
                next->setParent(cell);
                q.push_back(next);
            }
        }
    }
    return false;
}

bool Model::bestFirst() //pathfinding algorithm
{
    clearVisited();
    std::priority_queue <Cell*, QVector<Cell*>, comparatorBestFirst> pq;
    Cell *start = &_grid[_toRow][_toCol];
    Cell *goal = &_grid[_fromRow][_fromCol];
    int heuristic = abs(_fromCol - start->x()) + abs(_fromRow - start->y());
    start->setHeuristic(heuristic);
    start->setVisited(true);
    pq.push(start);

    while(!pq.empty())
    {
        Cell* cell = pq.top();
        pq.pop();
        if (cell == goal)
        {
            return true;
        }
        for (auto& next : neighbors(cell))
        {
            if( !next->visited())
            {
                int heuristic = abs(next->x() - _fromCol) + abs(next->y() - _fromRow);
                next->setVisited(true);
                next->setParent(cell);
                next->setHeuristic(heuristic);
                pq.push(next);
            }
        }
    }
    return false;
}

bool Model::aStar()
{
    clearMoveCost();

    std::priority_queue <Cell*, QVector<Cell*>, comparatorAStar> pq;
    Cell *start = &_grid[_toRow][_toCol];
    Cell * goal = &_grid[_fromRow][_fromCol];
    int heuristic = abs(_fromCol - start->x()) + abs(_fromRow - start->y());
    int moveCost = 1;
    start->setHeuristic(heuristic);
    start->setMoveCost(moveCost);
    pq.push(start);

    while(!pq.empty())
    {
        Cell* cell = pq.top();
        pq.pop();
        if (cell == goal)
        {
            return true;
        }

        for (auto& next : neighbors(cell))
        {
            moveCost ++;
            if( !next->moveCost() || ( next->moveCost() > moveCost) )
            {
                int heuristic = abs(next->x() - _fromCol) + abs(next->y() - _fromRow);
                next->setParent(cell);
                next->setHeuristic(heuristic);
                next->setMoveCost(moveCost);
                pq.push(next);
            }
        }
    }
    return false;
}

void Model::setScore(int score)
{
    _score = score;
    emit scoreChanged(_score);
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



int Model::addRandomFigures(int num)
{
    int emptyCellsCount = emptyCells();
    if (emptyCellsCount == 0)
    {
        return 0;
    }
    else if ( num > emptyCellsCount)
    {
        num = emptyCellsCount;
    }
    for (int i = 0 ; i < num; i++)
    {
        int r = 0;
        int c = 0;
        do {
            r = getRandomRow();
            c = getRandomCol();
        }
        while( _grid[r][c].cellType() != FigureType::EMPTY );
        _grid[r][c].setType(getRandomType());
        if (checkAndDeleteLines(r, c))
        {
            --i;
        }
    }
    if (emptyCells() == 0)
    {
        return 0;
    }
    else
    {
        return num;
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

void Model::addRandomFigure(int row, int col)
{
    _grid[row][col].setType(getRandomType());
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

int Model::emptyCells()
{
    int emptyCellsCount = 0;
    for (int r = 0; r < _row; r++)
    {
        for (int c = 0; c < _col; c++)
        {
            if (_grid[r][c].cellType() == FigureType::EMPTY)
            {
                emptyCellsCount++;
            }
        }
    }
    return emptyCellsCount;
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
    int bonus = deletedCells * (deletedCells - _equalCount + 1);
    _score += bonus;
    emit scoreChanged(_score);
}
