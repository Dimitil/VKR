#ifndef MODEL_H
#define MODEL_H

#include <QDebug>
#include <QObject>
#include <QColor>

#include <QVector>
#include <QSet>
#include <QQueue>
#include <queue>

#include "cell.h"
#include "comparators.h"

using Gridtype = QVector<QVector<Cell>>;

enum class DifficultyType{
    EAZY,
    NORMAL,
    HARD
};

class Model : public QObject
{
    Q_OBJECT

    int _col;
    int _row;
    int _equalCount;
    int _score;
    int _fromCol;
    int _fromRow;
    int _toCol;
    int _toRow;
    bool (Model::*_algorithm)();
    DifficultyType _difficulty;
    Gridtype _grid;

    void horizontalCheck(QSet<Cell*> &set, int row, int col);
    void verticalCheck(QSet<Cell*> &set, int row, int col);
    void rightDiagonalCheck(QSet<Cell*> &set, int row, int col);
    void leftDiagonalCheck(QSet<Cell*> &set, int row, int col);
    int deleteSet(QSet<Cell*> &set);
    int checkAndDeleteLines(int row, int col);

    void setScore(int score);
    void setEqualCount(int equalCount);
    void addScore(int score);
    void resize(int row, int col);

    void coordinateAllCells();
    void clear();
    void clearVisited();
    void clearMoveCost();
    void clearParent();

    int maxTypeCount();

    bool bfs(); //pathfinding algorithms
    bool bestFirst();
    bool aStar();
    QVector<Cell*> neighbors(Cell* cell);
    bool moveTo(int oldRow, int oldCol, int newRow, int newCol);

public:
    Model(QObject *parent = nullptr);

    void addRandomFigures(int num);
    void addFigures(int row, int col, FigureType figureType);
    void setFrom(int row, int col);
    void setTo(int row, int col);

    Gridtype& getGrid();
    int fromRow() const;
    int fromCol() const;
    int toRow() const;
    int toCol() const;
    int row() const;
    int col() const;
    int score() const;

    bool doStep();

signals:
    void scoreChanged(int newScore);
    void difficultyChanged();

public slots:
    void setBfsAlgorithm();
    void setBestFirstAlgorithm();
    void setAStarAlgorithm();

    void setEazyDifficulty();
    void setNormalDifficulty();
    void setHardDifficulty();
};

#endif // MODEL_H
