#ifndef POPOUT_WINDOW_H
#define POPOUT_WINDOW_H

#include <QWidget>
#include <QPoint>

#include "IPopOutWindow.h"
#include "IWindowWidget.h"

class PopOutWindow :  public IPopOutWindow
{
public:

    explicit PopOutWindow (QWidget *parent = nullptr, IWindowWidget *scrWidget = nullptr): screenWidget(scrWidget) {};

private:

    IWindowWidget *screenWidget = nullptr;
};

#endif // POPOUT_WINDOW_H
