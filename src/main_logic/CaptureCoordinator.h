#ifndef CARPTURECOORDINATOR_H
#define CARPTURECOORDINATOR_H

#include <QObject>
#include <memory>

#include "ICaptureCoordinator.h"
#include "screen_capturer.h"
#include "mouse_controller.h"


class CaptureCoordinatorLocal : public ICaptureCoordinator
{
    Q_OBJECT
public:
    explicit CaptureCoordinatorLocal(QObject *parent = nullptr);

    ScreenCapturer* screenCapturer() const;
    MouseController* mouseController() const;

private:
    std::unique_ptr<ScreenCapturer> m_capturer = nullptr;
    std::unique_ptr<MouseController> m_mouseController = nullptr;

};

#endif // CARPTURECOORDINATOR_H
