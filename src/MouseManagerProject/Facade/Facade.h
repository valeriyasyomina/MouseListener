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
    void ServerStarted() {emit ServerStartedSignal();}
    void ServerStopped() {emit ServerStoppedSignal();}
    void ErrorSendCommandToDevice() {emit ErrorSendCommandToDeviceSignal();}
    void ClientConnected(QString clientAddress, int clientPort) {emit ClientConnectedSignal(clientAddress, clientPort);}
    void ClientDisconnected(QString clientAddress, int clientPort) {emit ClientDisconnectedSignal(clientAddress, clientPort);}
signals:
    void ServerStartedSignal();
    void ServerStoppedSignal();
    void ErrorSendCommandToDeviceSignal();
    void ClientConnectedSignal(QString clientAddress, int clientPort);
    void ClientDisconnectedSignal(QString clientAddress, int clientPort);
};

#endif // FACADE_H
