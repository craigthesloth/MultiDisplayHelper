#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , screenCapturer(new ScreenCapturer(this))
    , mouseController(new MouseController(this))
    , screenWidget(new ScreenWidget(this))
{
    setupUI();
    setupConnections();
    updateScreenList();

    fpsSpinBox->setValue(40);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);


    QHBoxLayout *controlLayout = new QHBoxLayout();

    screenSelector = new QComboBox();
    fpsSpinBox = new QSpinBox();
    fpsSpinBox->setRange(1, 60);
    fpsSpinBox->setSuffix(" FPS");

    startButton = new QPushButton("Start Capture");
    stopButton = new QPushButton("Stop Capture");
    fullscreenButton = new QPushButton("Go Fullscreen");
    aboutButton = new QPushButton("About");
    statusLabel = new QLabel("Ready");
    fpsLabel = new QLabel("FPS: 0");

    controlLayout->addWidget(new QLabel("Screen:"));
    controlLayout->addWidget(screenSelector);
    controlLayout->addWidget(new QLabel("Target FPS:"));
    controlLayout->addWidget(fpsSpinBox);
    controlLayout->addWidget(startButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addWidget(fullscreenButton);
    controlLayout->addWidget(fpsLabel);
    controlLayout->addWidget(statusLabel);
    controlLayout->addStretch();
     controlLayout->addWidget(aboutButton);


    QWidget *controlWidget = new QWidget();
    controlWidget->setLayout(controlLayout);
    controlWidget->setFixedHeight(50);

    mainLayout->addWidget(controlWidget, 0);
    mainLayout->addWidget(screenWidget, 1);

    stopButton->setEnabled(false);


    screenSelector->setMaximumWidth(200);
    fpsSpinBox->setMaximumWidth(80);
    startButton->setFixedWidth(100);
    stopButton->setFixedWidth(100);
     fullscreenButton->setFixedWidth(100);
    statusLabel->setMinimumWidth(200);

    setWindowTitle("MultiDisplayHelper");
    resize(1000, 700);
}

void MainWindow::setupConnections()
{
    connect(screenCapturer, &ScreenCapturer::screenCaptured,
            this, &MainWindow::onScreenCaptured);
    connect(screenCapturer, &ScreenCapturer::fpsUpdated,
            this, &MainWindow::onFpsUpdated);

    connect(startButton, &QPushButton::clicked,
            this, &MainWindow::onStartCapture);

    connect(stopButton, &QPushButton::clicked,
            this, &MainWindow::onStopCapture);

    connect(screenSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onScreenSelected);

    connect(fpsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::onFpsChanged);

    connect(aboutButton, &QPushButton::clicked,
            this, &MainWindow::onAboutButton);

    connect(fullscreenButton, &QPushButton::clicked,
            this, &MainWindow::onFullscreenButton);


    connect(screenWidget, &ScreenWidget::mouseClicked,
            this, &MainWindow::onMouseClicked);

    connect(screenWidget, &ScreenWidget::mouseMoved,
            this, &MainWindow::onMouseMoved);

    connect(screenWidget, &ScreenWidget::mousePressed,
            this, &MainWindow::onMousePressed);

    connect(screenWidget, &ScreenWidget::mouseReleased,
            this, &MainWindow::onMouseReleased);

    connect(screenWidget, &ScreenWidget::mouseWheel,
            this, &MainWindow::onMouseWheel);
}

void MainWindow::updateScreenList()
{
    screenSelector->clear();
    QList<QScreen*> screens = QGuiApplication::screens();

    for (int i = 0; i < screens.size(); ++i) {
        QRect geometry = screens[i]->geometry();
        screenSelector->addItem(QString("Screen %1 - %2x%3 at %4,%5")
                                    .arg(i)
                                    .arg(geometry.width())
                                    .arg(geometry.height())
                                    .arg(geometry.x())
                                    .arg(geometry.y()));
    }

    if (screens.size() > 1) {
        screenSelector->setCurrentIndex(1);
    }
}

void MainWindow::onScreenCaptured(const QPixmap &pixmap)
{
    screenWidget->setScreenImage(pixmap);

    statusLabel->setText(QString("Capturing... %1x%2")
                             .arg(pixmap.width())
                             .arg(pixmap.height()));
}

void MainWindow::onFpsUpdated(int fps)
{
    fpsLabel->setText(QString("FPS: %1").arg(fps));
}

void MainWindow::onStartCapture()
{
    int screenIndex = screenSelector->currentIndex();

    if (screenCapturer->initialize(screenIndex) &&
        mouseController->initialize(screenIndex)) {

        screenCapturer->setTargetFps(fpsSpinBox->value());
        screenCapturer->startCapture();
        startButton->setEnabled(false);
        stopButton->setEnabled(true);

        statusLabel->setText(QString("Capturing screen %1 - %2 FPS")
                                 .arg(screenIndex)
                                 .arg(fpsSpinBox->value()));
    } else {
        statusLabel->setText("Failed to initialize capture");
    }
}

void MainWindow::onStopCapture()
{
    screenCapturer->stopCapture();
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
    fpsLabel->setText("FPS: 0");
    statusLabel->setText("Capture stopped");
}

void MainWindow::onScreenSelected(int index)
{
    Q_UNUSED(index);
    onStopCapture();
}



void MainWindow::onFpsChanged(int fps)
{
    screenCapturer->setTargetFps(fps);
    statusLabel->setText(QString("FPS set to: %1").arg(fps));
}

void MainWindow::onMouseClicked(const QPoint &position, Qt::MouseButton button)
{
    if (screenWidget->isCaptureActive()) {
        mouseController->sendMouseClick(position, button);
    }
}

void MainWindow::onMouseMoved(const QPoint &position)
{
    if (screenWidget->isCaptureActive()) {
        mouseController->sendMouseMove(position);
    }
}

void MainWindow::onMousePressed(const QPoint &position, Qt::MouseButton button)
{
    if (screenWidget->isCaptureActive()) {
        mouseController->sendMousePress(position, button);
    }
}

void MainWindow::onMouseReleased(const QPoint &position, Qt::MouseButton button)
{
    if (screenWidget->isCaptureActive()) {
        mouseController->sendMouseRelease(position, button);
    }
}

void MainWindow::onMouseWheel(const QPoint &position, int delta)
{
    if (screenWidget->isCaptureActive()) {
        mouseController->sendMouseWheel(position, delta);
    }
}

void MainWindow::onFullscreenButton()
{
    if (!isFullscreen) {
        showFullScreen();
        isFullscreen = true;
        fullscreenButton->setText("Exit Fullscreen");
    } else {
        showNormal();
        isFullscreen = false;
        fullscreenButton->setText("Go Fullscreen");
    }

}

void MainWindow::onAboutButton()
{
    QMessageBox::about(this, "About MultiDisplayHelper",
                       "MultiDisplay Helper\n\n"
                       "A tool for capturing and controlling multiple screens.\n"
                       "Made by P.Sobin"
                       "Version 1.02 BETA");
}
