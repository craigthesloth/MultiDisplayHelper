#ifndef ISCREENCAPTURER_H
#define ISCREENCAPTURER_H

#include <QObject>
#include <QPixmap>

#include "mercury.hpp"


class IScreenCapturer : public QObject {

    Q_OBJECT

public:
     explicit IScreenCapturer(QObject *parent = nullptr, Mercury::ThreadPool* p = nullptr) : QObject(parent) {}
    ~IScreenCapturer() override = default;

    virtual bool initialize(int screenIndex) = 0;
    virtual QPixmap captureScreen() = 0;


    virtual void setTargetFps(int fps) = 0;
    virtual int getCurrentFps() const = 0;

    virtual void setThreadPool(Mercury::ThreadPool* p = nullptr) = 0;


signals:

    void screenCaptured(const QPixmap &pixmap);
    void fpsUpdated(int fps);

public slots:
    virtual void startCapture() = 0;
    virtual void stopCapture() = 0;
    virtual void stopCaptureWithPool() = 0;
};

#endif // ISCREENCAPTURER_H
