#ifndef ALLOCMEMORYEXCEPTION_H
#define ALLOCMEMORYEXCEPTION_H

#include "Exception.h"

class AllocMemoryException : public Exception
{
public:
    explicit AllocMemoryException(std::string messageToSet)
    {
        message = messageToSet;
    }
    ~AllocMemoryException() throw (){}
};

#endif // ALLOCMEMORYEXCEPTION_H
