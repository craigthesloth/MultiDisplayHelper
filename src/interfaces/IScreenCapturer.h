#ifndef ISCREENCAPTURER_H
#define ISCREENCAPTURER_H

#include <QObject>
#include <QPixmap>


class IScreenCapturer : public QObject {
public:
     explicit IScreenCapturer(QObject *parent = nullptr) : QObject(parent) {}
    ~IScreenCapturer() override = default;

    virtual bool initialize(int screenIndex) = 0;
    virtual QPixmap captureScreen() = 0;


    virtual void setTargetFps(int fps) = 0;
    virtual int getCurrentFps() const = 0;

signals:

    virtual void screenCaptured(const QPixmap &pixmap) = 0;
    virtual void fpsUpdated(int fps) = 0;

public slots:
    virtual void startCapture() = 0;
    virtual void stopCapture() = 0;
};

#endif // ISCREENCAPTURER_H
