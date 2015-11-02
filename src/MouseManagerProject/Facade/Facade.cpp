#include "Facade/Facade.h"

Facade::Facade()
{
    mouseListener = new MouseListener();
    deviceManager = new DeviceManager();
}

Facade::~Facade()
{
    if (mouseListener)
    {
        delete mouseListener;
        mouseListener = NULL;
    }
    if (deviceManager)
    {
        delete deviceManager;
        deviceManager = NULL;
    }
}
