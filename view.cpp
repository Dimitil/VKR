#include "view.h"
#include "model.h"
#include <QPainter>
#include <chrono>
#include <thread>



void View::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawScene(&painter);
}

void View::drawScene(QPainter *painter)
{
    auto& grid = _model->getGrid();
    size_t col = grid.size();       //FIXME add getCol() and getRow() in model
    size_t row = grid[0].size();    //
    for (size_t r = 0; r < row; r++)
    {
        for( size_t c = 0; c < col; c++)
        {
            QRect rect = QRect(c*cellSize, r*cellSize, cellSize, cellSize);
            QRect miniRect = QRect(c*cellSize + cellSize/4,
                                   r*cellSize + cellSize/4,
                                   cellSize/2,
                                   cellSize/2);
            painter->setBrush(Qt::gray);
            painter->drawRect(rect);

            if (grid[r][c].cellType() == FigureType::ONE) {
                painter->setBrush(Qt::red);
                painter->drawEllipse(rect);
            }
            if (grid[r][c].cellType() == FigureType::TWO) {
                painter->setBrush(Qt::yellow);          //FIXME do function
                painter->drawEllipse(rect);
            }
            if (grid[r][c].cellType() == FigureType::THREE) {
                painter->setBrush(Qt::green);
                painter->drawEllipse(rect);
            }
            if (grid[r][c].cellType() == FigureType::FOUR) {
                painter->setBrush(Qt::blue);
                painter->drawEllipse(rect);
            }
            if (grid[r][c].cellType() == FigureType::FIVE) {
                painter->setBrush(Qt::black);
                painter->drawEllipse(miniRect);

            }
        }
    }
}

View::View(QWidget *parent) : QWidget(parent) { }

void View::setModel(Model *model)            { _model = model; }

void View::moveAnimation()
{
    auto& grid  = _model->getGrid();

    int fromRow = _model->fromRow();
    int fromCol = _model->fromCol();
    int toRow   = _model->toRow();
    int toCol   = _model->toCol();

    Cell* cell = &grid[fromRow][fromCol];
    FigureType ft = grid[toRow][toCol].cellType(); //потому что в модели
    while (!((cell->x() == toCol) &&                //перемещение уже произошло ко времени отрисовки
           (cell->y() == toRow)))
    {
        cell->setType(FigureType::FIVE);
        repaint();
        std::this_thread::sleep_for(std::chrono::milliseconds(400) );
        cell->setType(FigureType::EMPTY);
        cell = cell->cellParent();
        cell->setType(ft);
    }
}

void View::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::MouseButton::LeftButton )
    {
        int row = event->pos().y() / cellSize;
        int col = event->pos().x() / cellSize;
        _model->setFrom(row, col);
    }
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::MouseButton::LeftButton)
    {
        int row = event->pos().y() / cellSize;
        int col = event->pos().x() / cellSize;
        _model->setTo(row, col);
        if(_model->doStep())
        {
            moveAnimation();
        }
        repaint();
    }
}
