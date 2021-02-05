#include "view.h"
#include "model.h"

#include <QPainter>


void View::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawScene(&painter);
}

void View::drawScene(QPainter *painter)
{


    auto& grid = _model->getGrid();

    size_t col = grid.size();
    size_t row = grid[0].size();
    for (size_t r = 0; r < row; r++)
    {
        for( size_t c = 0; c < col; c++)
        {
            QRect rect = QRect(c*cellSize, r*cellSize, cellSize, cellSize);
            painter->setBrush(grid[r][c]);
            painter->drawRect(rect);
        }
    }
}
