#include "MouseHandlerWin.h"


#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QDebug>

void MouseHandlerWin::moveCursor(int x, int y)
{
    if (!SetCursorPos(x, y)) {
        qDebug() << "SetCursorPos failed, error:" << GetLastError();
    }
}

static DWORD mouseButtonDownFlag(Qt::MouseButton button)
{
    switch (button) {
    case Qt::LeftButton:   return MOUSEEVENTF_LEFTDOWN;
    case Qt::RightButton:  return MOUSEEVENTF_RIGHTDOWN;
    case Qt::MiddleButton: return MOUSEEVENTF_MIDDLEDOWN;
    default: return 0;
    }
}

static DWORD mouseButtonUpFlag(Qt::MouseButton button)
{
    switch (button) {
    case Qt::LeftButton:   return MOUSEEVENTF_LEFTUP;
    case Qt::RightButton:  return MOUSEEVENTF_RIGHTUP;
    case Qt::MiddleButton: return MOUSEEVENTF_MIDDLEUP;
    default: return 0;
    }
}

void MouseHandlerWin::pressButton(Qt::MouseButton button)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = mouseButtonDownFlag(button);
    if (input.mi.dwFlags)
        SendInput(1, &input, sizeof(INPUT));
}

void MouseHandlerWin::releaseButton(Qt::MouseButton button)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = mouseButtonUpFlag(button);
    if (input.mi.dwFlags)
        SendInput(1, &input, sizeof(INPUT));
}

void MouseHandlerWin::wheel(int delta)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.mouseData = delta;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    SendInput(1, &input, sizeof(INPUT));
}
