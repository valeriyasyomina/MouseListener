#ifndef SENDDEVICECOMMANDEXCEPTION_H
#define SENDDEVICECOMMANDEXCEPTION_H

#include "Exception.h"

class SendDeviceCommandException: public Exception
{
public:
    explicit SendDeviceCommandException(std::string messageToSet)
    {
        message = messageToSet;
    }
    ~SendDeviceCommandException() throw (){}
};

#endif // SENDDEVICECOMMANDEXCEPTION_H
