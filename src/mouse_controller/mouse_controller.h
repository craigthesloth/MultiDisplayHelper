#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QRect>
#include <QCursor>
#include <QGuiApplication>
#include <IMouseController.h>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class MouseController : public QObject, IMouseController
{
    Q_OBJECT

public:
    explicit MouseController(QObject *parent = nullptr);

    bool initialize(int targetScreenIndex) override;
    void sendMouseClick(const QPoint &position, Qt::MouseButton button = Qt::LeftButton) override;
    void sendMouseMove(const QPoint &position) override;
    void sendMousePress(const QPoint &position, Qt::MouseButton button = Qt::LeftButton) override;
    void sendMouseRelease(const QPoint &position, Qt::MouseButton button = Qt::LeftButton) override;
    void sendMouseWheel(const QPoint &position, int delta) override;

    QRect getScreenGeometry() const;
    QPoint getScreenOffset() const;

private:
    QPoint convertToVirtualDesktopCoordinates(const QPoint &screenLocalPos) const;

    QRect screenGeometry;
    int targetScreenIndex;
    QScreen *targetScreen;
};

#endif
