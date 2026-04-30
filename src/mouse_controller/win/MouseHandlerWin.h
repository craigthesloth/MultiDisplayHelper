#ifndef MOUSEHANDLERWIN_H
#define MOUSEHANDLERWIN_H

#include<QObject>
#include <QPoint>
#include "IMouseHandler.h"

class MouseHandlerWin : public IMouseHandler
{
public:
    MouseHandlerWin () = default;
    ~MouseHandlerWin () = default;


    void moveCursor(int x, int y) override;
    void pressButton(Qt::MouseButton button) override;
    void releaseButton(Qt::MouseButton button) override;
    void wheel(int delta) override;
};

#endif // MOUSEHANDLERWIN_H
