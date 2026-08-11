#ifndef CARPTURECOORDINATOR_H
#define CARPTURECOORDINATOR_H

#include <QObject>
#include <memory>

#include "ICaptureCoordinator.h"
#include "IMouseController.h"
#include "IScreenCapturer.h"


class CaptureCoordinatorLocal : public ICaptureCoordinator
{
    Q_OBJECT
public:
    explicit CaptureCoordinatorLocal(QObject* parent = nullptr);
    ~CaptureCoordinatorLocal() override = default;

    IScreenCapturer* screenCapturer() const override;
    IMouseController* mouseController() const override;

    bool startCapture(int screenIndex, int fps) override;
    void stopCapture() override;
    
    void setTargetFPS(int fps) override;
    bool isCapturing() const  override { return m_capturing; } ;

    QStringList avaitableScreens() const override;

    void setThreadPool(std::unique_ptr<Mercury::ThreadPool> p);
    void setThreadPool(Mercury::ThreadPool* extP);

private:
    std::unique_ptr<IScreenCapturer> m_capturer;
    std::unique_ptr<IMouseController> m_mouseController;
    std::unique_ptr<Mercury::ThreadPool> m_pool;
    Mercury::ThreadPool* ext_pool = nullptr;
    bool m_capturing = false;
};

#endif // CARPTURECOORDINATOR_H
