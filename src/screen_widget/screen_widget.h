#ifndef SCREEN_WIDGET_H
#define SCREEN_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>

#include "IWindowWidget.h"

class ScreenWidget : public IWindowWidget
{
    Q_OBJECT

public:
    explicit ScreenWidget(QWidget *parent = nullptr);

    void setScreenImage(const QPixmap &pixmap);
    qreal getScaleFactor() const;
    QPoint getImageOffset() const;
    bool isCaptureActive() const { return !screenPixmap.isNull(); }

    void updateRemoteCursorPosition();


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawRemoteCursor(QPainter &painter, const QPoint &position);
    QPoint convertWidgetToScreenPos(const QPoint &widgetPos) const;
    QPoint convertScreenToWidgetPos(const QPoint &screenPos) const;
    void updateScaleAndOffset();

    QPixmap screenPixmap;
    qreal scaleFactor;
    QSize originalSize;
    QPoint imageOffset;

    QPoint remoteCursorPos;
    QTimer* cursorUpdateTimer;

    //Drug'n'drop feature
    bool isDragging = false;
    Qt::MouseButton draggingButton = Qt::NoButton;
};

#endif
