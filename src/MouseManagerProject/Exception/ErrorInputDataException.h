#ifndef ERRORINPUTDATAEXCEPTION_H
#define ERRORINPUTDATAEXCEPTION_H

#include "Exception.h"

class ErrorInputDataException: public Exception
{
public:
    explicit ErrorInputDataException(std::string messageToSet)
    {
        message = messageToSet;
    }
    ~ErrorInputDataException() throw (){}
};

#endif // ERRORINPUTDATAEXCEPTION_H
