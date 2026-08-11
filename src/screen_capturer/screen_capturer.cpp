#include "screen_capturer.h"
#include <QDebug>
#include <QScreenCapture>
#include <QImage>

ScreenCapturer::ScreenCapturer(QObject *parent, Mercury::ThreadPool* p)
    : 
    IScreenCapturer(parent),
    targetScreen(nullptr),
    captureTimer(new QTimer(this)),
    targetFps(45),
    currentFps(0),
    frameCount(0),
    lastFpsUpdate(0),
    m_pool(p)
    
{
    captureTimer->setTimerType(Qt::PreciseTimer);
    connect(captureTimer, &QTimer::timeout, this, &ScreenCapturer::onCaptureTimeout);
    frameTimer.start();
    lastFpsUpdate = frameTimer.elapsed();

    // Определяем, нужно ли использовать Wayland/мультимедийный захват
    useWayland = QGuiApplication::platformName().contains("wayland", Qt::CaseInsensitive);
    if (useWayland) {
        waylandCapture = new QScreenCapture(this);

        // Create media capture session and video sink for receiving frames
        captureSession = new QMediaCaptureSession(this);
        videoSink = new QVideoSink(this);

        captureSession->setScreenCapture(waylandCapture);
        captureSession->setVideoSink(videoSink);

        // Connect to QVideoSink::videoFrameChanged
        connect(videoSink, &QVideoSink::videoFrameChanged,
                this, &ScreenCapturer::onFrameChanged);
    }
}

ScreenCapturer::~ScreenCapturer()
{
}

bool ScreenCapturer::initialize(int screenIndex)
{
    QList<QScreen*> screens = QGuiApplication::screens();
    if (screenIndex >= 0 && screenIndex < screens.size()) {
        targetScreen = screens[screenIndex];
        if (useWayland && waylandCapture) {
            waylandCapture->setScreen(targetScreen);
        }
        qDebug() << "Screen capturer initialized for screen" << screenIndex
                 << "with geometry:" << targetScreen->geometry()
                 << "(method:" << (useWayland ? "Wayland/QScreenCapture" : "X11/grabWindow") << ")";
        return true;
    }
    qDebug() << "Failed to initialize. Index:" << screenIndex;
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

int ScreenCapturer::getCurrentFps() const { return currentFps; }

QPixmap ScreenCapturer::captureScreen()
{
    if (!targetScreen) return QPixmap();
    return targetScreen->grabWindow(0);
}

void ScreenCapturer::startCapture()
{
    if (!targetScreen) return;

    if (useWayland && waylandCapture) {
        startWaylandCapture();
    } else {
        int intervalMs = 1000 / targetFps;
        captureTimer->start(intervalMs);
        qDebug() << "Screen capture started (grabWindow) with" << targetFps << "FPS, interval:" << intervalMs << "ms";
    }
    frameTimer.restart();
    frameCount = 0;
    lastFpsUpdate = frameTimer.elapsed();
}

void ScreenCapturer::stopCapture()
{
    if (useWayland && waylandCapture) {
        stopWaylandCapture();
    }
    captureTimer->stop();
    currentFps = 0;
    qDebug() << "Screen capture stopped";
}

void ScreenCapturer::onCaptureTimeout()
{
    QPixmap pixmap = captureScreen();
    if (!pixmap.isNull()) {
        emit screenCaptured(pixmap);
        updateFpsCounter();
    } else {
        qWarning() << "grabWindow returned null pixmap!";
    }
}

// ------------ Wayland / QScreenCapture -------------------

void ScreenCapturer::startWaylandCapture()
{
    if (waylandCapture) {
        waylandCapture->start();
        qDebug() << "Wayland screen capture started (QScreenCapture)";
    }
}

void ScreenCapturer::stopWaylandCapture()
{
    if (waylandCapture)
        waylandCapture->stop();
}

void ScreenCapturer::onFrameChanged(QVideoFrame frame)
{
    // При Wayland мы получаем кадры от QScreenCapture.
    // Этот слот вызывается в GUI-потоке.
    if (frame.isValid()) {
        QImage image = frame.toImage();
        if (!image.isNull()) {
            QPixmap pix = QPixmap::fromImage(image);
            emit screenCaptured(pix);
           updateFpsCounter();
        }
    }
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