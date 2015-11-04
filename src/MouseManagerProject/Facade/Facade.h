#ifndef FACADE_H
#define FACADE_H

#include "DeviceManager/DeviceManager.h"
#include "MouseListener/MouseListener.h"
#include <QObject>

class Facade: public QObject
{
    Q_OBJECT
private:
    MouseListener* mouseListener;
    DeviceManager* deviceManager;
public:
    Facade();
    ~Facade();

    MouseListener* GetMouseListener() {return mouseListener;}
    DeviceManager* GetDeviceManager() {return deviceManager;}
};

#endif // FACADE_H
