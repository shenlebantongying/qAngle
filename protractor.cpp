#include "protractor.h"
#include <QList>
#include <QtMath>
#include <cmath>
#include <QMouseEvent>

const int HANDLE_W = 20;

protractor::protractor(QWidget *) {
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    setFixedSize(400, 400);
    handle_A = QRect(370, 200 - HANDLE_W / 2, HANDLE_W, HANDLE_W);
    handle_B = QRect(50, 50, HANDLE_W, HANDLE_W);
}

void protractor::paintEvent(QPaintEvent *inEvent) {
    Q_UNUSED(inEvent);

    int w = this->width();
    int r = w / 2;
    auto pen = new QPen(Qt::black,2);
    QPainter painter(this);
    painter.setPen(*pen);
    painter.setRenderHint(QPainter::Antialiasing);

    pen->setColor(Qt::green);
    painter.setPen(*pen);
    painter.drawLine(0,200,400,200);
    painter.drawLine(200,0,200,400);

    pen->setColor(Qt::red);
    painter.setPen(*pen);
    painter.drawLine(QPoint(r, r), handle_A.center());
    painter.drawLine(QPoint(r, r), handle_B.center());

    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(handle_A);
    painter.drawRect(handle_B);

    // TODO: revise math here
    int x1 = handle_A.center().x() - this->width() / 2;
    int y1 = -(handle_A.center().y() - this->width() / 2);
    int x2 = handle_B.center().x() - this->width() / 2;
    int y2 = -(handle_B.center().y() - this->width() / 2);
    int dot = x1 * x2 + y1 * y2;
    int det = x1 * y2 - y1 * x2;

    this->windowHandle()->setTitle(QString::number(round(qRadiansToDegrees(qAbs(std::atan2(det, dot)))))+QString("°"));
}


void protractor::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event);

    QPoint p = event->position().toPoint();

    if (!this->geometry().contains(p)){
        event->accept();
        return;
    }

    if (handle_A_MOVE) {
        handle_A.moveCenter(p);

    } else if (handl_B_MOVE) {
        handle_B.moveCenter(p);
    } else if (window_MOVE) {
        this->windowHandle()->startSystemMove();
    }

    event->accept();

    update();
}

void protractor::mousePressEvent(QMouseEvent *ev) {
    if (handle_A.contains(ev->position().toPoint())) {
        handle_A_MOVE = true;
    } else if (handle_B.contains(ev->position().toPoint())) {
        handl_B_MOVE = true;
    } else {
        window_MOVE = true;
    };
    ev->accept();
}

void protractor::mouseReleaseEvent(QMouseEvent *event) {
    handle_A_MOVE = false;
    handl_B_MOVE = false;
    window_MOVE = false;
    event->accept();
}
