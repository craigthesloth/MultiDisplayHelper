#ifndef ISCREENCAPTURER_H
#define ISCREENCAPTURER_H

#include <QObject>
#include <QScreen>
#include <QPixmap>
#include <QTimer>
#include <QGuiApplication>
#include <QElapsedTimer>

class IScreenCapturer {
public:
    IScreenCapturer() = default;
    ~IScreenCapturer() = default;

    virtual bool initialize(int screenIndex);
    virtual QPixmap captureScreen();

};

#endif // ISCREENCAPTURER_H
