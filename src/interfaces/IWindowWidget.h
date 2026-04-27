#ifndef IWINDOWWIDGET_H
#define IWINDOWWIDGET_H

#include <QWidget>

class IWindowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IWindowWidget(QWidget *parent = nullptr): QWidget(parent) {};
    ~IWindowWidget() = default;

signals:
    void mouseClicked(const QPoint &position, Qt::MouseButton button);
    void mouseMoved(const QPoint &position);
    void mouseDragged(const QPoint &position, Qt::MouseButton button );
    void mousePressed(const QPoint &position, Qt::MouseButton button);
    void mouseReleased(const QPoint &position, Qt::MouseButton button);
    void mouseWheel(const QPoint &position, int delta);

};


#endif // IWINDOWWIDGET_H
