#ifndef IMOUSEHANDLER_H
#define IMOUSEHANDLER_H

#include <QObject>

class IMouseHandler
{
public:
    IMouseHandler() = default;
    virtual ~IMouseHandler() = default;

    virtual void moveCursor(int x, int y) = 0;
    virtual void pressButton(Qt::MouseButton button) = 0;
    virtual void releaseButton(Qt::MouseButton button) = 0;
    virtual void wheel(int delta) = 0;
};

#endif // IMOUSEHANDLER_H
