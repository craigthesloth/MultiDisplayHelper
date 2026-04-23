#ifndef SCREEN_CAPTURER_H
#define SCREEN_CAPTURER_H

#include <QObject>
#include <QScreen>
#include <QPixmap>
#include <QTimer>
#include <QGuiApplication>
#include <QElapsedTimer>

class ScreenCapturer : public QObject
{
    Q_OBJECT

public:
    explicit ScreenCapturer(QObject *parent = nullptr);
    ~ScreenCapturer();

    bool initialize(int screenIndex = 1);
    QPixmap captureScreen();

    void setTargetFps(int fps);
    int getCurrentFps() const;

public slots:
    void startCapture();
    void stopCapture();

signals:
    void screenCaptured(const QPixmap &pixmap);
    void fpsUpdated(int fps);

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
