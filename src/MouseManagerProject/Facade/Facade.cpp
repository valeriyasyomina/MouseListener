#include "Facade/Facade.h"


Facade::Facade()
{
    mouseListener = new MouseListener();
    deviceManager = new DeviceManager();

    connect(mouseListener, SIGNAL(MessageReceived(const char*)),deviceManager, SLOT(SendCommandToDevice(const char*)));
    connect(deviceManager, SIGNAL(KernelModuleInserted()),mouseListener, SLOT(DeviceKernelModuleInserted()));
    connect(deviceManager, SIGNAL(KernelModuleRemoved()),mouseListener, SLOT(DeviceKernelModuleRemoved()));
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
