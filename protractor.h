#pragma once

#include <QWidget>

class protractor : public QWidget {
    Q_OBJECT

    const static int handle_apothem = 11;
    const static int handle_width = handle_apothem * 2;

public:
    explicit protractor(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent*) override;

private:
    // Line's P1 is the center dot, P2 is the handle's position
    QLineF A;
    QLineF B;

    bool handle_A_MOVE = false;
    bool handl_B_MOVE = false;

    bool cursor_on_A = false;
    bool cursor_on_B = false;

    [[nodiscard]] int halfHeight() const;
    [[nodiscard]] int halfWidth() const;

    [[nodiscard]] QPoint getCenterPoint() const;
    [[nodiscard]] double getAngleBetweenLines();

    [[nodiscard]] auto getHandleRect(QPoint point) -> QRect;
    [[nodiscard]] QPoint calcApproximateHandlePoint(QPoint point);

protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};
