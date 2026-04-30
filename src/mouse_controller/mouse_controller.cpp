#include "mouse_controller.h"
#include <QDebug>
#include <QScreen>

#ifdef Q_OS_WIN
#include "MouseHandlerWin.h"
#else
#include "MouseHandlerLinux.h"
#endif

MouseController::MouseController(QObject *parent)
    : IMouseController(parent)
    , targetScreenIndex(-1)
    , targetScreen(nullptr)
{
#ifdef Q_OS_WIN
    platformMouse = std::make_unique<MouseHandlerWin>();
#else
    platformMouse = std::make_unique<MouseHandlerLinux>();
#endif
}

bool MouseController::initialize(int screenIndex)
{
    QList<QScreen*> screens = QGuiApplication::screens();

    if (screenIndex >= 0 && screenIndex < screens.size()) {
        targetScreenIndex = screenIndex;
        targetScreen = screens[screenIndex];
        screenGeometry = targetScreen->geometry();

        qDebug() << "Initialized mouse controller for screen" << screenIndex
                 << "Geometry:" << screenGeometry
                 << "Available screens:" << screens.size();

        return true;
    }

    qDebug() << "Failed to initialize mouse controller. Screen index:" << screenIndex;
    return false;
}

QRect MouseController::getScreenGeometry() const
{
    return screenGeometry;
}

QPoint MouseController::getScreenOffset() const
{
    return screenGeometry.topLeft();
}

QPoint MouseController::convertToVirtualDesktopCoordinates(const QPoint &screenLocalPos) const
{
    if (!targetScreen) return screenLocalPos;

    QPoint virtualPos = QPoint(screenGeometry.x() + screenLocalPos.x(),
                               screenGeometry.y() + screenLocalPos.y());

    qDebug() << "Coordinate conversion - Local:" << screenLocalPos
             << "Screen geometry:" << screenGeometry
             << "Virtual:" << virtualPos;

    return virtualPos;
}

void MouseController::sendMouseClick(const QPoint &position, Qt::MouseButton button)
{
    sendMousePress(position, button);
    sendMouseRelease(position, button);
}

void MouseController::sendMouseMove(const QPoint &position)
{
    QPoint vPos = convertToVirtualDesktopCoordinates(position);
    platformMouse->moveCursor(vPos.x(), vPos.y());
}

void MouseController::sendMousePress(const QPoint &position, Qt::MouseButton button)
{
    QPoint vPos = convertToVirtualDesktopCoordinates(position);
    platformMouse->moveCursor(vPos.x(), vPos.y());
    platformMouse->pressButton(button);
}

void MouseController::sendMouseRelease(const QPoint &position, Qt::MouseButton button)
{
    QPoint vPos = convertToVirtualDesktopCoordinates(position);
    platformMouse->moveCursor(vPos.x(), vPos.y());
    platformMouse->releaseButton(button);
}

void MouseController::sendMouseWheel(const QPoint &position, int delta)
{
    QPoint vPos = convertToVirtualDesktopCoordinates(position);
    platformMouse->moveCursor(vPos.x(), vPos.y());
    platformMouse->wheel(delta);
}
