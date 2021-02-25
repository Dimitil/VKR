#include "view.h"
#include "model.h"
#include <QPainter>
#include <chrono>
#include <thread>



void View::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawScene(&painter);
    event->accept();
}

void View::drawScene(QPainter *painter)
{
    auto& grid = _model->getGrid();
    size_t col = _model->col();
    size_t row = _model->row();
    for (size_t r = 0; r < row; r++)
    {
        for( size_t c = 0; c < col; c++)
        {
            drawFigure(grid[r][c], painter);
        }
    }
}

View::View(QWidget *parent) : QWidget(parent) { }

void View::setModel(Model *model) { _model = model; }

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
        std::this_thread::sleep_for(std::chrono::milliseconds(100) );
        cell->setType(FigureType::EMPTY);
        cell = cell->cellParent();
        cell->setType(ft);
    }
}

void View::drawFigure(Cell cell, QPainter *painter)
{
    int col = cell.x();
    int row = cell.y();
    QRect rect = QRect(col*cellSize, row*cellSize, cellSize, cellSize);
    painter->setBrush(Qt::gray);
    painter->drawRect(rect);
    switch(cell.cellType())
    {
        case FigureType::EMPTY:
            break;
        case FigureType::ONE:
            painter->setBrush(Qt::red);
            painter->drawEllipse(rect);
            break;
        case FigureType::TWO:
            painter->setBrush(Qt::yellow);
            painter->drawEllipse(rect);
            break;
        case FigureType::THREE:
            painter->setBrush(Qt::blue);
            painter->drawEllipse(rect);
            break;
        case FigureType::FOUR:
            painter->setBrush(Qt::green);
            painter->drawEllipse(rect);
            break;
        case FigureType::FIVE:
            QRect miniRect = QRect(col*cellSize + cellSize/4,
                               row*cellSize + cellSize/4,
                               cellSize/2,
                               cellSize/2);
            painter->setBrush(Qt::black);
            painter->drawEllipse(miniRect);
            break;
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
