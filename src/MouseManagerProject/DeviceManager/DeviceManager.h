#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <fcntl.h>
#include <unistd.h>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "Exception/AllocMemoryException.h"
#include "Exception/ShellCommandExecuteException.h"
#include "Exception/NullInputDataException.h"
#include "Exception/ErrorInputDataException.h"
#include <Exception/FileException.h>
#include <Exception/SendDeviceCommandException.h>
#include <iostream>
#include <fstream>



class DeviceManager : public QObject
{
    Q_OBJECT
private:
    int deviceFileDescriptor;
    std::string loadedModuleName;
public:
    DeviceManager(): deviceFileDescriptor(0) {}
    ~DeviceManager() {}

    void OpenDeviceFile(const std::string& fileName);
    void CloseDeviceFile();
    void InsertDeviceModule(const std::string& modulePath);
    void RemoveDeviceModule();
public slots:
    void SendCommandToDevice(const char* command);
private:
    void GetModuleNameFromPath(const std::string& modulePath);
signals:
    void KernelModuleInserted();
    void KernelModuleRemoved();
};

#endif // DEVICEMANAGER_H
