#include "CaptureCoordinator.h"
#include "screen_capturer.h"
#include "mouse_controller.h"

CaptureCoordinatorLocal::CaptureCoordinatorLocal(QObject* parent)
    : ICaptureCoordinator(parent)
{
    m_capturer = std::make_unique<ScreenCapturer>(nullptr, nullptr);
    m_mouseController = std::make_unique<MouseController>(nullptr);
}

IScreenCapturer* CaptureCoordinatorLocal::screenCapturer() const
{
    return m_capturer.get();
}

IMouseController* CaptureCoordinatorLocal::mouseController() const
{
    return m_mouseController.get();
}

bool CaptureCoordinatorLocal::startCapture(int screenIndex, int fps)
{
    if (!m_capturer || !m_mouseController) return m_capturing = false;
    if (!m_capturer->initialize(screenIndex) || !m_mouseController->initialize(screenIndex)) return m_capturing = false;
    
    return m_capturing = true;
}

void CaptureCoordinatorLocal::stopCapture() {
    if(m_capturer)
    {
        m_capturer->stopCapture();
        if (auto* sc = dynamic_cast<IScreenCapturer*>(m_capturer.get()))
            sc->stopCaptureWithPool();
    }
    m_capturing = false;
}

void CaptureCoordinatorLocal::setTargetFPS(int fps)
{
    if (m_capturer) m_capturer->setTargetFps(fps);
}

QStringList CaptureCoordinatorLocal::avaitableScreens() const
{
    QStringList list;
    const auto screens = QGuiApplication::screens();
    for(int i = 0; i< screens.size(); ++i)
    {
        QRect geo = screens[i]->geometry();
        list << QString("Screen %1 - %2x%3 at %4,%5")
            .arg(i).arg(geo.width()).arg(geo.height()).arg(geo.x()).arg(geo.y());
    }
    return list;
}


void CaptureCoordinatorLocal::setThreadPool(std::unique_ptr<Mercury::ThreadPool> p)
{
    m_pool = std::move(p);
    ext_pool = m_pool.get();
    if (auto* sc = dynamic_cast<IScreenCapturer*>(m_capturer.get())) sc->setThreadPool(ext_pool);
}

void CaptureCoordinatorLocal::setThreadPool(Mercury::ThreadPool* ext)
{
    ext_pool = ext;
    if (auto* sc = dynamic_cast<IScreenCapturer*>(m_capturer.get())) sc->setThreadPool(ext_pool);
}