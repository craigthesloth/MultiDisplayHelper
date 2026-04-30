#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <memory>
#include "IMouseController.h"
#include "IMouseHandler.h"

class MouseController : public IMouseController
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

    QRect getScreenGeometry() const override;
    QPoint getScreenOffset() const override;

private:
    QPoint convertToVirtualDesktopCoordinates(const QPoint &screenLocalPos) const;

    QRect screenGeometry;
    int targetScreenIndex = -1;
    QScreen *targetScreen = nullptr;
    std::unique_ptr<IMouseHandler> platformMouse;
};

#endif
