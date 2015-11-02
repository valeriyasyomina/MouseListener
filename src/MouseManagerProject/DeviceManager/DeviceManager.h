#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <Exception/OpenFileException.h>
#include <Exception/SendDeviceCommandException.h>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class DeviceManager : public QObject
{
    Q_OBJECT
private:
    int deviceFileDescriptor;
public:
    DeviceManager() {}
    ~DeviceManager() {}

    void OpenDeviceFile(const char* fileName);
    void CloseDeviceFile();
    void SendCommandToDevice(const char* command);
};

#endif // DEVICEMANAGER_H
