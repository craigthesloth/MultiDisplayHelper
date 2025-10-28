#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QRect>
#include <QCursor>
#include <QGuiApplication>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class MouseController : public QObject
{
    Q_OBJECT

public:
    explicit MouseController(QObject *parent = nullptr);

    bool initialize(int targetScreenIndex);
    void sendMouseClick(const QPoint &position, Qt::MouseButton button = Qt::LeftButton);
    void sendMouseMove(const QPoint &position);
    void sendMousePress(const QPoint &position, Qt::MouseButton button = Qt::LeftButton);
    void sendMouseRelease(const QPoint &position, Qt::MouseButton button = Qt::LeftButton);
    void sendMouseWheel(const QPoint &position, int delta);

    QRect getScreenGeometry() const;
    QPoint getScreenOffset() const;

private:
    QPoint convertToVirtualDesktopCoordinates(const QPoint &screenLocalPos) const;

    QRect screenGeometry;
    int targetScreenIndex;
    QScreen *targetScreen;
};

#endif
