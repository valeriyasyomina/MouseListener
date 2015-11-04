#include "DeviceManager.h"

///
/// \brief DeviceManager::OpenDeviceFile Открывает файл устройтсва по заданному имени
/// \param fileName Имя файла
///
void DeviceManager::OpenDeviceFile(const std::string& fileName)
{
    deviceFileDescriptor = open(fileName.c_str(),O_RDWR);
    if (deviceFileDescriptor < 0)
    {
        throw FileException("Error open file: no such file! Kernel module was not loaded!");
    }
}

///
/// \brief DeviceManager::CloseDeviceFile   Закрывает файл устройства по его дескриптору
///
void DeviceManager::CloseDeviceFile()
{
    if (close(deviceFileDescriptor) < 0)
        throw FileException("Error close device file!");
}

///
/// \brief DeviceManager::SendCommandToDevice  Отправляет команду устройству путем записи в файл устройства
/// \param command  Команда
///
void DeviceManager::SendCommandToDevice(const char* command)
{
    qDebug()<< "In device manager";
    qDebug()<< command;
    int commandSendRes = write(deviceFileDescriptor, command, strlen(command));
    if (commandSendRes < 0)
        throw SendDeviceCommandException("Error send command to device!");
}
///
/// \brief InsertDeviceModule  Вгружает загружаемый модуль устройства в ядро
/// \param modulePath   Путь к загружаемому модулю
///
void DeviceManager::InsertDeviceModule(const std::string& modulePath)
{
    try
    {
        std::ifstream inputFile(modulePath.c_str());
        if (!inputFile.is_open())
            throw FileException("Kernel module with such name doesn't exist!");

        inputFile.close();
        GetModuleNameFromPath(modulePath);
        std::string command = "sudo insmod " + modulePath;

        int returnValue = system(command.c_str());
        if (returnValue == -1 || WEXITSTATUS(returnValue) != 0)
            throw ShellCommandExecuteException("Kernel module has already been loaded!");
    }
    catch (std::bad_alloc& exception)
    {
        throw AllocMemoryException("Error alloc memory in InsertDeviceModule");
    }
}

///
/// \brief RemoveDeviceModule Выгружает загружаемый модуль устройства из ядра
///
void DeviceManager::RemoveDeviceModule()
{
    std::string command = "sudo rmmod " + loadedModuleName;
    int returnValue = system(command.c_str());
    if (returnValue == -1 || WEXITSTATUS(returnValue) != 0)
        throw ShellCommandExecuteException("Kernel module was already removed or not loaded yet");
}

///
/// \brief DeviceManager::GetModuleNameFromPath Выделяет имя загружаемого модуля из пути к модулю
/// \param modulePath  Путь к загружаемому модулю
///
void DeviceManager::GetModuleNameFromPath(const std::string& modulePath)
{
    try
    {
        if (!modulePath.length())
            throw NullInputDataException("No input data!");
        int position = modulePath.length() - 1;
        while (position >= 0 && modulePath[position] != '/')
            position--;
        if (position < 0)
            throw ErrorInputDataException("Input data has incorrect format!");
        loadedModuleName = modulePath.substr(position + 1, modulePath.length() - position + 1);
    }
    catch (std::bad_alloc& exception)
    {
        throw AllocMemoryException("Error alloc memory in GetModuleNameFromPath");
    }
}
