#ifndef FACADE_H
#define FACADE_H

#include "DeviceManager/DeviceManager.h"
#include "MouseListener/MouseListener.h"

class Facade
{
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
