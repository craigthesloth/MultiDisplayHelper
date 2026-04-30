#ifndef IPOPOUTWINDOW_H
#define IPOPOUTWINDOW_H

#include <QWidget>
#include "IWindowWidget.h"

class IPopOutWindow : public QWidget
{
public:
    explicit IPopOutWindow(QWidget *parent = nullptr, IWindowWidget *scrWidget = nullptr): QWidget(parent), screenWidget(scrWidget) {};
    ~IPopOutWindow() = default;

protected:
    IWindowWidget *screenWidget = nullptr;
};

#endif // IPOPOUTWINDOW_H
