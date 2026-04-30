#ifndef SCREEN_CAPTURER_H
#define SCREEN_CAPTURER_H

#include <QScreen>
#include <QPixmap>
#include <QTimer>
#include <QGuiApplication>
#include <QElapsedTimer>
#include <QVideoFrame>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include "IScreenCapturer.h"

QT_BEGIN_NAMESPACE
class QScreenCapture;
QT_END_NAMESPACE

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
    void onFrameChanged(QVideoFrame frame);

private:
    void updateFpsCounter();
    void startWaylandCapture();
    void stopWaylandCapture();

    QScreen *targetScreen = nullptr;
    QTimer *captureTimer = nullptr;
    QElapsedTimer frameTimer;
    int targetFps = 45;
    int currentFps = 0;
    int frameCount = 0;
    qint64 lastFpsUpdate = 0;

    // Wayland / мультимедийный захват
    QScreenCapture *waylandCapture = nullptr;
    bool useWayland = false;
    QMediaCaptureSession *captureSession = nullptr;
    QVideoSink *videoSink = nullptr;
};

#endif