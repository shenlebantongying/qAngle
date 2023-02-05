#ifndef CIRCLE_H
#define CIRCLE_H

#include <QWidget>
#include <QWindow>
#include <QPainter>
#include <QLayout>
#include <QWindow>

class protractor : public QWidget
{
    Q_OBJECT
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
public:
    explicit protractor(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *) override;

private:
    QRect handle_A;
    QRect handle_B;

    bool handle_A_MOVE = false;
    bool handl_B_MOVE = false;

    bool window_MOVE = false;

};

#endif // CIRCLE_H
