#ifndef COMPARATORS_H
#define COMPARATORS_H
#include "cell.h"

struct comparatorBestFirst{
    bool operator()(const Cell* l, const Cell* r)
    {
        return l->heuristic() > r->heuristic();
    };
};

struct comparatorAStar{
    bool operator()(const Cell* l, const Cell* r)
    {
        int lSum = l->heuristic() + l->moveCost();
        int rSum = r->heuristic() + r->moveCost();
        return lSum > rSum;
    }
};

#endif // COMPARATORS_H
