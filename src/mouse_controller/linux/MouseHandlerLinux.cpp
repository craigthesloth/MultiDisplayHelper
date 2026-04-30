#include "MouseHandlerLinux.h"
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>


static Display* getDisplay()
{
    if (auto *x11App = qGuiApp->nativeInterface<QNativeInterface::QX11Application>())
        return x11App->display();
    return nullptr;
}

void MouseHandlerLinux::moveCursor(int x, int y)
{
    Display *display = getDisplay();
    if (!display) {
        qWarning() << "X11 display not available";
        return;
    }
    XTestFakeMotionEvent(display, -1, x, y, 0);
    XFlush(display);
}

static unsigned int qtToXButton(Qt::MouseButton button)
{
    switch (button) {
    case Qt::LeftButton:   return Button1;
    case Qt::MiddleButton: return Button2;
    case Qt::RightButton:  return Button3;
    default: return 0;
    }
}

void MouseHandlerLinux::pressButton(Qt::MouseButton button)
{
    Display *display = getDisplay();
    if (!display) return;
    unsigned int xbtn = qtToXButton(button);
    if (xbtn) {
        XTestFakeButtonEvent(display, xbtn, True, 0);
        XFlush(display);
    }
}

void MouseHandlerLinux::releaseButton(Qt::MouseButton button)
{
    Display *display = getDisplay();
    if (!display) return;
    unsigned int xbtn = qtToXButton(button);
    if (xbtn) {
        XTestFakeButtonEvent(display, xbtn, False, 0);
        XFlush(display);
    }
}

void MouseHandlerLinux::wheel(int delta)
{
    Display *display = getDisplay();
    if (!display) return;
    unsigned int btn = (delta > 0) ? Button4 : Button5;
    int clicks = qAbs(delta) / 120;
    for (int i = 0; i < clicks; ++i) {
        XTestFakeButtonEvent(display, btn, True, 0);
        XTestFakeButtonEvent(display, btn, False, 0);
    }
    XFlush(display);
}
