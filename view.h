#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>


class Model;
class Cell;

class View : public QWidget
{
    Q_OBJECT
    static constexpr size_t cellSize = 50;
    Model *_model;

    void moveAnimation();

    void drawFigure(Cell cell, QPainter *painter);
protected:
    void drawScene(QPainter *painter);

public:
    View(QWidget *parent = nullptr);
    void setModel(Model *model);


protected:

    void paintEvent(QPaintEvent *event)         override;
    void mousePressEvent(QMouseEvent *event)    override;
    void mouseReleaseEvent(QMouseEvent *event)  override;

public slots:
    void setFixedSize();
    void gameOverDialog();
};

#endif // VIEW_H
