#include "Facade/Facade.h"


Facade::Facade()
{
    mouseListener = new MouseListener();
    deviceManager = new DeviceManager();

    connect(mouseListener, SIGNAL(MessageReceived(const char*)),deviceManager, SLOT(SendCommandToDevice(const char*)));
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
