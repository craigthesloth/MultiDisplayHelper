#ifndef POPOUT_WINDOW_H
#define POPOUT_WINDOW_H

#include <QWidget>
#include <QPoint>

#include "IPopOutWindow.h"
#include "screen_widget.h"

class PopOutWindow :  public IPopOutWindow
{
public:
    PopOutWindow();
};

#endif // POPOUT_WINDOW_H
