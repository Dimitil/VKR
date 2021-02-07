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
    size_t col = grid.size();       //FIXME add getCol() and getRow() in model
    size_t row = grid[0].size();
    for (size_t r = 0; r < row; r++)
    {
        for( size_t c = 0; c < col; c++)
        {
            QRect rect = QRect(c*cellSize, r*cellSize, cellSize, cellSize);
            painter->setBrush(Qt::gray);
            painter->drawRect(rect);

            if (grid[r][c] == 1) {
                painter->setBrush(Qt::red);
                painter->drawEllipse(rect);
            }
            if (grid[r][c] == 2) {
                painter->setBrush(Qt::yellow);          //FIXME do function
                painter->drawEllipse(rect);
            }
            if (grid[r][c] == 3) {
                painter->setBrush(Qt::green);
                painter->drawEllipse(rect);
            }
            if (grid[r][c] == 4) {
                painter->setBrush(Qt::blue);
                painter->drawEllipse(rect);
            }
        }
    }
}
