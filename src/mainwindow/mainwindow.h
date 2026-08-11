#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>

#include "IScreenCapturer.h"
#include "IMouseController.h"
#include "screen_widget.h"

#include "../lib/mercury.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, Mercury::ThreadPool* p = nullptr);
    ~MainWindow();

    void init();

private slots:
    void onScreenCaptured(const QPixmap &pixmap);
    void onFpsUpdated(int fps);

    void onStartCapture();
    void onStopCapture();
    void onFullscreenButton();
    void onAboutButton();

    void onScreenSelected(int index);
    void onFpsChanged(int fps);

    void onMouseClicked(const QPoint &position, Qt::MouseButton button);
    void onMouseMoved(const QPoint &position);
    void onMousePressed(const QPoint &position, Qt::MouseButton button);
    void onMouseReleased(const QPoint &position, Qt::MouseButton button);
    void onMouseWheel(const QPoint &position, int delta);



private:
    void setupUI();
    void setupConnections();
    void updateScreenList();

    IScreenCapturer *screenCapturer = nullptr;
    IMouseController *mouseController = nullptr;
    ScreenWidget *screenWidget;

    QComboBox *screenSelector;
    QSpinBox *fpsSpinBox;

    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *fullscreenButton;
    QPushButton *aboutButton;

    QLabel *statusLabel;
    QLabel *fpsLabel;

    bool isFullscreen = false;

    Mercury::ThreadPool* m_threadpool = nullptr;
};

#endif
