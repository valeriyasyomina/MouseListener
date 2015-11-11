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
public slots:
    void ServerStarted(QString serverAddress, int serverPort) {emit ServerStartedSignal(serverAddress, serverPort);}
    void ServerStopped() {emit ServerStoppedSignal();}
    void ErrorSendCommandToDevice() {emit ErrorSendCommandToDeviceSignal();}
signals:
    void ServerStartedSignal(QString serverAddress, int serverPort);
    void ServerStoppedSignal();
    void ErrorSendCommandToDeviceSignal();
};

#endif // FACADE_H
