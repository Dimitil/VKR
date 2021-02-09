#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

class Model;

class View : public QWidget
{
    Q_OBJECT
    Model *_model;

    bool figureSelected = false;
    size_t _rowBuf;
    size_t _colBuf;

protected:
    static constexpr size_t cellSize = 50;

    virtual void paintEvent(QPaintEvent *event)         override;
    void drawScene(QPainter *painter);


public:
    View(QWidget *parent = nullptr);
    void setModel(Model *model);


protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // VIEW_H
