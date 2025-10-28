#include "screen_capturer.h"
#include <QDebug>

ScreenCapturer::ScreenCapturer(QObject *parent)
    : QObject(parent),
    targetScreen(nullptr),
    captureTimer(new QTimer(this)),
    targetFps(45),
    currentFps(0),
    frameCount(0),
    lastFpsUpdate(0)
{
    captureTimer->setTimerType(Qt::PreciseTimer);
    connect(captureTimer, &QTimer::timeout, this, &ScreenCapturer::onCaptureTimeout);
    frameTimer.start();
    lastFpsUpdate = frameTimer.elapsed();
}

ScreenCapturer::~ScreenCapturer()
{
    stopCapture();
}

bool ScreenCapturer::initialize(int screenIndex)
{
    QList<QScreen*> screens = QGuiApplication::screens();

    if (screenIndex >= 0 && screenIndex < screens.size()) {
        targetScreen = screens[screenIndex];
        qDebug() << "Screen capturer initialized for screen" << screenIndex
                 << "with geometry:" << targetScreen->geometry();
        return true;
    }

    qDebug() << "Failed to initialize screen capturer. Screen index:" << screenIndex;
    return false;
}

void ScreenCapturer::setTargetFps(int fps)
{
    targetFps = qBound(1, fps, 60);
    qDebug() << "Target FPS set to:" << targetFps;

    if (captureTimer->isActive()) {
        stopCapture();
        startCapture();
    }
}

int ScreenCapturer::getCurrentFps() const
{
    return currentFps;
}

QPixmap ScreenCapturer::captureScreen()
{
    if (!targetScreen) return QPixmap();

     return targetScreen->grabWindow(0);
}

void ScreenCapturer::startCapture()
{
    if (targetScreen) {
        int intervalMs = 1000 / targetFps;
        captureTimer->start(intervalMs);
        frameTimer.restart();
        frameCount = 0;
        lastFpsUpdate = frameTimer.elapsed();

        qDebug() << "Screen capture started with" << targetFps << "FPS (interval:" << intervalMs << "ms)";
    }
}

void ScreenCapturer::stopCapture()
{
    captureTimer->stop();
    currentFps = 0;
    qDebug() << "Screen capture stopped";
}

void ScreenCapturer::onCaptureTimeout()
{
    QPixmap pixmap = captureScreen();
    emit screenCaptured(pixmap);
    updateFpsCounter();
}

void ScreenCapturer::updateFpsCounter()
{
    frameCount++;
    qint64 currentTime = frameTimer.elapsed();
    qint64 elapsed = currentTime - lastFpsUpdate;

    if (elapsed >= 1000) {
        currentFps = qRound((frameCount * 1000.0) / elapsed);
        emit fpsUpdated(currentFps);

        frameCount = 0;
        lastFpsUpdate = currentTime;

          static int debugCounter = 0;
        if (++debugCounter >= 5) {
            qDebug() << "Capture FPS:" << currentFps << "/" << targetFps;
            debugCounter = 0;
        }
    }
}
