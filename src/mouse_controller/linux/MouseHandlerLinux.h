#ifndef MOUSEHANDLERLINUX_H
#define MOUSEHANDLERLINUX_H

#include "IMouseHandler.h"

#include <QDebug>
#include <QtGlobal>
#include <QGuiApplication>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>


class MouseHandlerLinux : public IMouseHandler
{
public:
    MouseHandlerLinux () = default;
    ~MouseHandlerLinux () = default;



    void moveCursor(int x, int y) override;
    void pressButton(Qt::MouseButton button) override;
    void releaseButton(Qt::MouseButton button) override;
    void wheel(int delta) override;

};

#endif // MOUSEHANDLERLINUX_H
