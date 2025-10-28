#include "mouse_controller.h"
#include <QDebug>
#include <QScreen>

MouseController::MouseController(QObject *parent)
    : QObject(parent), targetScreenIndex(-1), targetScreen(nullptr)
{
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
    QPoint virtualPos = convertToVirtualDesktopCoordinates(position);

    qDebug() << "Moving mouse to:" << virtualPos << "(local:" << position << ")";

#ifdef Q_OS_WIN
    if (SetCursorPos(virtualPos.x(), virtualPos.y())) {

    } else {
        qDebug() << "SetCursorPos failed. Error:" << GetLastError();
    }
#endif
}

void MouseController::sendMousePress(const QPoint &position, Qt::MouseButton button)
{
    QPoint virtualPos = convertToVirtualDesktopCoordinates(position);

#ifdef Q_OS_WIN

    SetCursorPos(virtualPos.x(), virtualPos.y());

    INPUT input = {0};
    input.type = INPUT_MOUSE;

    if (button == Qt::LeftButton) {
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    } else if (button == Qt::RightButton) {
        input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    } else if (button == Qt::MiddleButton) {
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
    }

    SendInput(1, &input, sizeof(INPUT));
#endif
}

void MouseController::sendMouseRelease(const QPoint &position, Qt::MouseButton button)
{
    QPoint virtualPos = convertToVirtualDesktopCoordinates(position);

#ifdef Q_OS_WIN
   SetCursorPos(virtualPos.x(), virtualPos.y());

    INPUT input = {0};
    input.type = INPUT_MOUSE;

    if (button == Qt::LeftButton) {
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    } else if (button == Qt::RightButton) {
        input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    } else if (button == Qt::MiddleButton) {
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
    }

    SendInput(1, &input, sizeof(INPUT));
#endif
}

void MouseController::sendMouseWheel(const QPoint &position, int delta)
{
    QPoint virtualPos = convertToVirtualDesktopCoordinates(position);

#ifdef Q_OS_WIN
    SetCursorPos(virtualPos.x(), virtualPos.y());

    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.mouseData = delta;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;

    SendInput(1, &input, sizeof(INPUT));
#endif
}
