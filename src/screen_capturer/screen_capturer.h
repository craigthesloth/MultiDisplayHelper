#ifndef SCREEN_CAPTURER_H
#define SCREEN_CAPTURER_H

#include <QScreen>
#include <QPixmap>
#include <QTimer>
#include <QGuiApplication>
#include <QElapsedTimer>
#include "IScreenCapturer.h"

class ScreenCapturer : public IScreenCapturer
{
    Q_OBJECT

public:
    explicit ScreenCapturer(QObject *parent = nullptr);
    ~ScreenCapturer();

    bool initialize(int screenIndex = 1) override;
    QPixmap captureScreen() override;
    void setTargetFps(int fps) override;
    int getCurrentFps() const override;

public slots:
    void startCapture() override;
    void stopCapture() override;

private slots:
    void onCaptureTimeout();

private:
    void updateFpsCounter();

    QScreen *targetScreen;
    QTimer *captureTimer;
    QElapsedTimer frameTimer;
    int targetFps;
    int currentFps;
    int frameCount;
    qint64 lastFpsUpdate;
};

#endif
