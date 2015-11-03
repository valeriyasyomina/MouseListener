#ifndef SOCKETREADDATAEXCEPTION_H
#define SOCKETREADDATAEXCEPTION_H

#include "Exception.h"

class SocketReadDataException: public Exception
{
public:
    explicit SocketReadDataException(std::string messageToSet)
    {
        message = messageToSet;
    }
    ~SocketReadDataException() throw (){}
};


#endif // SOCKETREADDATAEXCEPTION_H
