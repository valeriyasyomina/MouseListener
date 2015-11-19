#include "Facade/Facade.h"
#include "Exception/AllocMemoryException.h"

Facade::Facade()
{
    try
    {
        mouseListener = new MouseListener();
        deviceManager = new DeviceManager();

        connect(mouseListener, SIGNAL(MessageReceivedSignal(const char*)),deviceManager, SLOT(SendCommandToDevice(const char*)));
        connect(mouseListener, SIGNAL(ServerStartedSignal()), this, SLOT(ServerStarted()));
        connect(mouseListener, SIGNAL(ServerStoppedSignal()), this, SLOT(ServerStopped()));
        connect(mouseListener, SIGNAL(ClientConnectedSignal(QString,int)), this, SLOT(ClientConnected(QString,int)));
        connect(mouseListener, SIGNAL(ClientDisconnectedSignal(QString,int)), this, SLOT(ClientDisconnected(QString,int)));
        connect(mouseListener, SIGNAL(ClientBroadCastSignal(QString,int)), this, SLOT(ClientBroadCast(QString,int)));

        connect(deviceManager, SIGNAL(KernelModuleInsertedSignal()),mouseListener, SLOT(DeviceKernelModuleInserted()));
        connect(deviceManager, SIGNAL(KernelModuleRemovedSignal()),mouseListener, SLOT(DeviceKernelModuleRemoved()));
        connect(deviceManager, SIGNAL(ErrorSendCommandToDeviceSignal()), this, SLOT(ErrorSendCommandToDevice()));
    }
    catch (std::bad_alloc& exception)
    {
        throw AllocMemoryException("Error allocate memory in Facade::Facade()");
    }
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
