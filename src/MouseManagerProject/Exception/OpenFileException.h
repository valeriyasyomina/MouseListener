#ifndef OPENFILEEXCEPTION_H
#define OPENFILEEXCEPTION_H

#include <Exception/Exception.h>

class OpenFileException: public Exception
{
public:
    explicit OpenFileException(std::string messageToSet)
    {
        message = messageToSet;
    }
    ~OpenFileException() throw (){}
};

#endif // OPENFILEEXCEPTION_H
