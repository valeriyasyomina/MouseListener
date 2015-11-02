#include "DeviceManager.h"

///
/// \brief DeviceManager::OpenDeviceFile Открывает файл устройтсва по заданному имени
/// \param fileName Имя файла
///
void DeviceManager::OpenDeviceFile(const char* fileName)
{
    deviceFileDescriptor = open(fileName,O_RDWR);
    if (deviceFileDescriptor < 0)
    {
        throw new OpenFileException("Error open device file!");
    }
}

///
/// \brief DeviceManager::CloseDeviceFile   Закрывает файл устройства по его дескриптору
///
void DeviceManager::CloseDeviceFile()
{
    close(deviceFileDescriptor);
}

///
/// \brief DeviceManager::SendCommandToDevice  Отправляет команду устройству путем записи в файл устройства
/// \param command  Команда
///
void DeviceManager::SendCommandToDevice(const char* command)
{
    int commandSendRes = write(deviceFileDescriptor, command, strlen(command));
    if (commandSendRes < 0)
    {
        throw new SendDeviceCommandException("Error send command to device!");
    }
}
