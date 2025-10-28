#include "screen_widget.h"
#include <QDebug>
#include <QApplication>
#include <QGuiApplication>

ScreenWidget::ScreenWidget(QWidget *parent)
    : QWidget(parent),
    scaleFactor(1.0),
    imageOffset(0, 0),
    remoteCursorPos(0, 0),
    cursorUpdateTimer(new QTimer(this))
{
    setMouseTracking(true);

     cursorUpdateTimer->setInterval(50);
    connect(cursorUpdateTimer, &QTimer::timeout, this, &ScreenWidget::updateRemoteCursorPosition);
}

void ScreenWidget::setScreenImage(const QPixmap &pixmap)
{
    screenPixmap = pixmap;
    originalSize = pixmap.size();

     if (!pixmap.isNull() && remoteCursorPos.isNull()) {
        remoteCursorPos = QPoint(pixmap.width() / 2, pixmap.height() / 2);
        cursorUpdateTimer->start();
    } else if (pixmap.isNull()) {
        cursorUpdateTimer->stop();
    }

    updateScaleAndOffset();
    update();
}

void ScreenWidget::updateRemoteCursorPosition()
{
    QPoint globalPos = QCursor::pos();

     QScreen* currentScreen = QGuiApplication::screenAt(globalPos);
    if (!currentScreen) return;

      QRect screenGeometry = currentScreen->geometry();
    if (screenGeometry.contains(globalPos)) {
         remoteCursorPos = QPoint(globalPos.x() - screenGeometry.x(),
                                 globalPos.y() - screenGeometry.y());
        update();
    }
}

qreal ScreenWidget::getScaleFactor() const
{
    return scaleFactor;
}

QPoint ScreenWidget::getImageOffset() const
{
    return imageOffset;
}

void ScreenWidget::updateScaleAndOffset()
{
    if (screenPixmap.isNull()) return;

    qreal scaleX = qreal(width()) / qreal(screenPixmap.width());
    qreal scaleY = qreal(height()) / qreal(screenPixmap.height());
    scaleFactor = qMin(scaleX, scaleY);

    int scaledWidth = screenPixmap.width() * scaleFactor;
    int scaledHeight = screenPixmap.height() * scaleFactor;

    imageOffset.setX((width() - scaledWidth) / 2);
    imageOffset.setY((height() - scaledHeight) / 2);
}

void ScreenWidget::drawRemoteCursor(QPainter &painter, const QPoint &position)
{
    painter.setRenderHint(QPainter::Antialiasing, true);

    int size = 8;

    painter.setBrush(QColor(0, 100, 255, 200));
    painter.setPen(QPen(Qt::white, 2));
    painter.drawEllipse(position, size, size);

    painter.setBrush(QColor(255, 255, 255, 180));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(position, size/2, size/2);
}

void ScreenWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(45, 45, 48));

    if (!screenPixmap.isNull()) {
        QPixmap scaledPixmap = screenPixmap.scaled(
            screenPixmap.size() * scaleFactor,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );

        painter.drawPixmap(imageOffset, scaledPixmap);

        QPoint widgetCursorPos = convertScreenToWidgetPos(remoteCursorPos);
        if (rect().contains(widgetCursorPos)) {
            drawRemoteCursor(painter, widgetCursorPos);
        }

        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 10));
        painter.drawText(10, 25, QString("Scale: %1").arg(scaleFactor, 0, 'f', 2));
        painter.drawText(10, 45, QString("Remote cursor: %1, %2").arg(remoteCursorPos.x()).arg(remoteCursorPos.y()));
        painter.drawText(10, 65, "Click to move cursor to this position");
    } else {
        painter.fillRect(rect(), QColor(60, 60, 60));
        painter.setPen(QColor(200, 200, 200));
        painter.setFont(QFont("Arial", 14, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "No screen image available\nStart capture to begin");
    }
}

QPoint ScreenWidget::convertWidgetToScreenPos(const QPoint &widgetPos) const
{
    if (screenPixmap.isNull()) return QPoint();

    int screenX = (widgetPos.x() - imageOffset.x()) / scaleFactor;
    int screenY = (widgetPos.y() - imageOffset.y()) / scaleFactor;

    screenX = qBound(0, screenX, screenPixmap.width() - 1);
    screenY = qBound(0, screenY, screenPixmap.height() - 1);

    return QPoint(screenX, screenY);
}

QPoint ScreenWidget::convertScreenToWidgetPos(const QPoint &screenPos) const
{
    if (screenPixmap.isNull()) return QPoint();

    int widgetX = imageOffset.x() + screenPos.x() * scaleFactor;
    int widgetY = imageOffset.y() + screenPos.y() * scaleFactor;

    return QPoint(widgetX, widgetY);
}

void ScreenWidget::mousePressEvent(QMouseEvent *event)
{
    if (screenPixmap.isNull()) {
        event->ignore();
        return;
    }

    QPoint screenPos = convertWidgetToScreenPos(event->pos());

     emit mouseMoved(screenPos);

    emit mousePressed(screenPos, event->button());

    event->accept();
}

void ScreenWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (screenPixmap.isNull()) {
        event->ignore();
        return;
    }

    QPoint screenPos = convertWidgetToScreenPos(event->pos());
    emit mouseReleased(screenPos, event->button());

    event->accept();
}

void ScreenWidget::mouseMoveEvent(QMouseEvent *event)
{
    update();
    event->accept();
}

void ScreenWidget::wheelEvent(QWheelEvent *event)
{
    if (screenPixmap.isNull()) {
        event->ignore();
        return;
    }

    QPoint screenPos = convertWidgetToScreenPos(event->position().toPoint());
    int delta = event->angleDelta().y();
    emit mouseWheel(screenPos, delta);

    event->accept();
}

void ScreenWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    updateScaleAndOffset();
}
