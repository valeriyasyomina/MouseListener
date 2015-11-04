#ifndef NULLINPUTDATAEXCEPTION_H
#define NULLINPUTDATAEXCEPTION_H

#include "Exception.h"

class NullInputDataException: public Exception
{
public:
    explicit NullInputDataException(std::string messageToSet)
    {
        message = messageToSet;
    }
    ~NullInputDataException() throw (){}
};

#endif // NULLINPUTDATAEXCEPTION_H
