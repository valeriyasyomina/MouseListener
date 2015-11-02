#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string.h>
#include <new>

class Exception
{
protected:
    char* message;
public:
    Exception() {}
    Exception(char* messageToSet)
    {
        int messageLength = strlen(messageToSet);
        message = new char[messageLength + 1];
        memmove(message, messageToSet, messageLength + 1);
    }
    virtual ~Exception()
    {
        if (message)
        {
            delete [] message;
            message = NULL;
        }
    }

    char* GetMessage() const {return message;}
};

#endif // EXCEPTION_H
