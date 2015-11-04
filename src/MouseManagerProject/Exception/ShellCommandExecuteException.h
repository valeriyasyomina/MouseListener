#ifndef SHELLCOMMANDEXECUTEEXCEPTION_H
#define SHELLCOMMANDEXECUTEEXCEPTION_H

#include "Exception.h"

class ShellCommandExecuteException: public Exception
{
public:
    explicit ShellCommandExecuteException(std::string messageToSet)
    {
        message = messageToSet;
    }
    ~ShellCommandExecuteException() throw (){}
};

#endif // SHELLCOMMANDEXECUTEEXCEPTION_H
