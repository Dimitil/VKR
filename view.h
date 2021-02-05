#ifndef VIEW_H
#define VIEW_H

#include <QWidget>


class Model;
class Controller;

class View : public QWidget
{
    Q_OBJECT
    Model *_model;
    Controller *_controller;

protected:
    static constexpr size_t cellSize = 50;

    virtual void paintEvent(QPaintEvent *event)         override;
    void drawScene(QPainter *painter);

public:
    View(QWidget *parent = nullptr) : QWidget(parent) { }
    void setModel(Model *model)            { _model = model; }
    void setController(Controller *controller)                     { _controller = controller; }

};

#endif // VIEW_H
