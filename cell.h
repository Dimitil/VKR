#ifndef CELL_H
#define CELL_H

enum class FigureType : char
{
    EMPTY,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    MAX,
};

class Cell{
    FigureType _type;
    Cell* _parent = nullptr;
    bool _visited = false;
    int _heuristic;
    int _moveCost;
    int _y;
    int _x;

public:
    Cell();
    Cell(FigureType type);
    int y() const;
    int x() const;
    FigureType cellType() const;
    bool visited() const;
    Cell* cellParent() const;
    int heuristic() const;
    int moveCost() const;
    void setY(int y);
    void setX(int x);
    void setVisited(bool b);
    void setType(FigureType type);
    void setParent(Cell* newParent);
    void setHeuristic(int heuristic);
    void setMoveCost(int moveCost);
};

#endif // CELL_H
