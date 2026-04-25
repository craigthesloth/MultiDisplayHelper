#ifndef IMOUSECONTROLLER_H
#define IMOUSECONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QRect>
#include <QCursor>
#include <QGuiApplication>

class IMouseController : public QObject {

    Q_OBJECT

public:
    explicit IMouseController(QObject *parent = nullptr) : QObject(parent){};
    ~IMouseController() = default;

    virtual bool initialize(int targetScreenindex) = 0;
    virtual void sendMouseClick(const QPoint &position, Qt::MouseButton button = Qt::LeftButton) = 0;
    virtual void sendMouseMove(const QPoint &position) = 0;
    virtual void sendMousePress(const QPoint &position, Qt::MouseButton button = Qt::LeftButton) = 0;
    virtual void sendMouseRelease(const QPoint &position, Qt::MouseButton button = Qt::LeftButton) = 0;
    virtual void sendMouseWheel(const QPoint &position, int delta)= 0;

    virtual QRect getScreenGeometry() const = 0;
    virtual QPoint getScreenOffset() const = 0;

};


#endif // IMOUSECONTROLLER_H
