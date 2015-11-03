#ifndef CLOSEFILEEXCEPTION_H
#define CLOSEFILEEXCEPTION_H

#include "Exception.h"

class CloseFileException: public Exception
{
public:
    explicit CloseFileException(std::string messageToSet)
    {
        message = messageToSet;
    }
    ~CloseFileException() throw (){}
};

#endif // CLOSEFILEEXCEPTION_H
