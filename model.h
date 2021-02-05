#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QColor>

class Model : QObject
{
    Q_OBJECT

    QVector<QVector<QColor>> _grid;

public:
    Model(size_t col = 10, size_t row = 10);

    QVector<QVector<QColor>>& getGrid() { return _grid;}
};

#endif // MODEL_H
