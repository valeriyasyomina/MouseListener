#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H

#include "Exception.h"

class FileException: public Exception
{
public:
    explicit FileException(std::string messageToSet)
    {
        message = messageToSet;
    }
    ~FileException() throw (){}
};

#endif // FILEEXCEPTION_H
