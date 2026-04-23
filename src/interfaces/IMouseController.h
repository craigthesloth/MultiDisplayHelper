#ifndef IMOUSECONTROLLER_H
#define IMOUSECONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QRect>
#include <QCursor>
#include <QGuiApplication>

class IMouseController{
public:
    IMouseController() = default;
    ~IMouseController() = default;

    virtual bool initialize(int targetScreenindex);
    virtual void sendMouseClick(const QPoint &position, Qt::MouseButton button = Qt::LeftButton);
    virtual void sendMouseMove(const QPoint &position);
    virtual void sendMousePress(const QPoint &position, Qt::MouseButton button = Qt::LeftButton);
    virtual void sendMouseRelease(const QPoint &position, Qt::MouseButton button = Qt::LeftButton);
    virtual void sendMouseWheel(const QPoint &position, int delta);

    virtual QRect getScreenGeometry();
    virtual QPoint getScreenOffset();

};


#endif // IMOUSECONTROLLER_H
