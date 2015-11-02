#ifndef SENDDEVICECOMMANDEXCEPTION_H
#define SENDDEVICECOMMANDEXCEPTION_H

#include "Exception/Exception.h"

class SendDeviceCommandException: public Exception
{
public:
    SendDeviceCommandException(char* messageToSet)
    {
        int messageLength = strlen(messageToSet);
        message = new char[messageLength + 1];
        memmove(message, messageToSet, messageLength + 1);
    }
    ~SendDeviceCommandException()
    {
        int a = 0;
        a = 10;
    }
};

#endif // SENDDEVICECOMMANDEXCEPTION_H
