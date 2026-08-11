#ifndef ICAPTURECONTROLLER_H
#define ICAPTURECONTROLLER_H

#include <QObject>
#include "IScreenCapturer.h"
#include "IMouseController.h"

class ICaptureCoordinator : public QObject
{
    Q_OBJECT
public:
    explicit ICaptureCoordinator(QObject *parent = nullptr) : QObject(parent) {};
    virtual ~ICaptureCoordinator() = default;

    virtual IScreenCapturer* screenCapturer() const = 0;
    virtual IMouseController* mouseController() const = 0;

    virtual bool startCapture(int screenIndex, int fps) = 0;
    virtual void stopCapture() = 0;
    virtual void setTargetFPS(int fps) = 0;
    virtual bool isCapturing() const = 0;
    
    virtual QStringList avaitableScreens() const = 0;
};


#endif // ICAPTURECONTROLLER_H
