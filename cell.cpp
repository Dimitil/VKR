#include "cell.h"


int Cell::y() const
{
    return _y;
}

int Cell::x() const
{
    return _x;
}

void Cell::setY(int y)
{
    _y = y;
}

void Cell::setX(int x)
{
    _x = x;
}

bool Cell::visited() const
{
    return _visited;
}

void Cell::setVisited(bool b)
{
    _visited = b;
}

void Cell::setType(FigureType type)
{
    _type = type;
}

void Cell::setParent(Cell *newParent)
{
    _parent = newParent;
}

void Cell::setHeuristic(int heuristic)
{
    _heuristic = heuristic;
}

void Cell::setMoveCost(int moveCost)
{
    _moveCost = moveCost;
}

Cell *Cell::cellParent() const
{
    return _parent;
}

int Cell::heuristic() const
{
    return _heuristic;
}

int Cell::moveCost() const
{
    return _moveCost;
}

Cell::Cell(FigureType type) : _type(type) {}

FigureType Cell::cellType() const
{
    return _type;
}

Cell::Cell() : _type(FigureType::EMPTY), _parent(nullptr){}
