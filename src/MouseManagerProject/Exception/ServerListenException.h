#ifndef SERVERLISTENEXCEPTION_H
#define SERVERLISTENEXCEPTION_H

#include "Exception.h"

class ServerListenException: public Exception
{
public:
    explicit ServerListenException(std::string messageToSet)
    {
        message = messageToSet;
    }
    ~ServerListenException() throw (){}
};

#endif // SERVERLISTENEXCEPTION_H
