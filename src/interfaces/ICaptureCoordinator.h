#ifndef ICAPTURECONTROLLER_H
#define ICAPTURECONTROLLER_H

#include <QObject>

class ICaptureCoordinator : public QObject
{
    Q_OBJECT
public:
    explicit ICaptureCoordinator(QObject *parent = nullptr) : QObject(parent) {};
    virtual ~ICaptureCoordinator() = default;


};


#endif // ICAPTURECONTROLLER_H
