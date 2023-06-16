#include "protractor.h"
#include <QGuiApplication>
#include <QLayout>
#include <QList>
#include <QMouseEvent>
#include <QPainter>
#include <QWindow>
#include <QtMath>

protractor::protractor(QWidget*)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    setFixedSize(400, 400);
    A = QLineF(getCenterPoint(), QPoint(int(width() * 0.25), int(height() * 0.25)));
    B = QLineF(getCenterPoint(), QPoint(int(width() * 0.75), int(height() * 0.5)));
}

void protractor::paintEvent(QPaintEvent* inEvent)
{
    Q_UNUSED(inEvent);

    auto const static alpha = 240;
    auto const static highLightColor = QColor(51, 166, 184); // Asagi-iro

    int w = this->width();
    int r = w / 2;
    auto pen = new QPen(Qt::black, 2);
    QPainter painter(this);
    painter.setPen(*pen);
    painter.setRenderHint(QPainter::Antialiasing);

    pen->setColor(QColor(0, 255, 0, alpha));
    painter.setPen(*pen);

    auto cp = getCenterPoint();

    // middle +
    painter.drawLine(QPoint(cp.x() - 20, cp.y()), QPoint(cp.x() + 20, cp.y())); // vertical
    painter.drawLine(QPoint(halfWidth(), cp.y() - 20), QPoint(halfWidth(), cp.y() + 20)); // horizontal

    // 4 ticks around the circle
    // TODO: more ticks?
    // up right down left
    painter.drawLine(QPoint(halfWidth(), 0), QPoint(halfWidth(), 20));
    painter.drawLine(QPoint(width() - 20, cp.y()), QPoint(width(), cp.y()));
    painter.drawLine(QPoint(halfWidth(), height()), QPoint(halfWidth(), height() - 20));
    painter.drawLine(QPoint(0, cp.y()), QPoint(20, cp.y()));

    painter.drawEllipse(geometry());

    pen->setColor(QColor(255, 0, 0, alpha));
    painter.setPen(*pen);
    painter.drawLine(QPoint(r, r), A.p2());
    painter.drawLine(QPoint(r, r), B.p2());

    pen->setColor(Qt::black);
    painter.setPen(*pen);

    if (handle_A_MOVE || handl_B_MOVE) {
        painter.setBrush(Qt::white);
        painter.drawEllipse(getHandleRect(A.p2().toPoint()));
        painter.drawEllipse(getHandleRect(B.p2().toPoint()));
    } else if (cursor_on_A && !handle_A_MOVE) {
        painter.setBrush(highLightColor);
        painter.drawEllipse(getHandleRect(A.p2().toPoint()));
        painter.setBrush(Qt::white);
        painter.drawEllipse(getHandleRect(B.p2().toPoint()));
    } else if (cursor_on_B && !handl_B_MOVE) {
        painter.setBrush(Qt::white);
        painter.drawEllipse(getHandleRect(A.p2().toPoint()));
        painter.setBrush(highLightColor);
        painter.drawEllipse(getHandleRect(B.p2().toPoint()));
    } else {
        painter.setBrush(Qt::white);
        painter.drawEllipse(getHandleRect(A.p2().toPoint()));
        painter.drawEllipse(getHandleRect(B.p2().toPoint()));
    }
}

int protractor::halfWidth() const { return width() / 2; }

int protractor::halfHeight() const { return height() / 2; }

void protractor::mouseMoveEvent(QMouseEvent* event)
{
    QPoint p = event->position().toPoint();

    if (getHandleRect(A.p2().toPoint()).contains(p)) {
        cursor_on_A = true;
        cursor_on_B = false;
    } else if (getHandleRect(B.p2().toPoint()).contains(p)) {
        cursor_on_A = false;
        cursor_on_B = true;
    } else {
        cursor_on_A = false;
        cursor_on_B = false;
    }

    if (!this->geometry().contains(p)) {
        return;
    }

    if (!handle_A_MOVE && !handl_B_MOVE) {
        if (QLineF(p, getCenterPoint()).length() > halfWidth()) {
            this->windowHandle()->startSystemMove();
        }
    } else {
        // move 5 degree is shift is hold
        if (QGuiApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)) {
            if (handle_A_MOVE) {
                A.setP2(calcApproximateHandlePoint(p));
            } else if (handl_B_MOVE) {
                B.setP2(calcApproximateHandlePoint(p));
            }
        } else {
            if (handle_A_MOVE) {
                A.setP2(p);
            } else if (handl_B_MOVE) {
                B.setP2(p);
            }
        }
        this->windowHandle()->setTitle(QString::number(getAngleBetweenLines()) + QString("°"));
    }

    update();
    event->accept();
}

void protractor::mousePressEvent(QMouseEvent* ev)
{
    if (cursor_on_A) {
        handle_A_MOVE = true;
    } else if (cursor_on_B) {
        handl_B_MOVE = true;
    }
    ev->accept();
}

void protractor::mouseReleaseEvent(QMouseEvent* event)
{
    handle_A_MOVE = false;
    handl_B_MOVE = false;
    event->accept();
}

double protractor::getAngleBetweenLines()
{
    // TODO: revise math here
    auto x1 = int(A.p2().x() - this->width() / 2.0);
    auto y1 = int(-(A.p2().y() - this->width() / 2.0));
    auto x2 = int(B.p2().x() - this->width() / 2.0);
    auto y2 = int(-(B.p2().y() - this->width() / 2.0));
    int dot = x1 * x2 + y1 * y2;
    int det = x1 * y2 - y1 * x2;

    return round(qRadiansToDegrees(qAbs(std::atan2(det, dot))));
}
QPoint protractor::getCenterPoint() const
{
    return { halfWidth(), halfHeight() };
}

QPoint protractor::calcApproximateHandlePoint(QPoint point)
{
    double angle = QLineF(getCenterPoint(), point).angle();
    int newAngle = ((int)std::round(angle / 5)) * 5;

    QLineF tempLine = QLineF::fromPolar(QLineF(getCenterPoint(), point).length(), newAngle);
    tempLine.translate(getCenterPoint());

    return tempLine.p2().toPoint();
}
QRect protractor::getHandleRect(QPoint point)
{
    return { point.x() - handle_apothem, point.y() - handle_apothem, handle_width, handle_width };
}
