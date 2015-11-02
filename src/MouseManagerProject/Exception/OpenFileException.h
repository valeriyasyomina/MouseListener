#ifndef OPENFILEEXCEPTION_H
#define OPENFILEEXCEPTION_H

#include <Exception/Exception.h>

class OpenFileException: public Exception
{
public:
    OpenFileException(char* messageToSet)
    {
        int messageLength = strlen(messageToSet);
        message = new char[messageLength + 1];
        memmove(message, messageToSet, messageLength + 1);
    }
    ~OpenFileException() {}
};

#endif // OPENFILEEXCEPTION_H
