#include "model.h"

Model::Model(size_t col, size_t row)
{
    for(size_t i = 0; i < row; i++) {
        _grid.push_back(QVector<QColor>(col, Qt::gray));
    }
}
