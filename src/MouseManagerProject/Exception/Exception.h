#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <new>

class Exception : public std::exception
{
protected:
    std::string message;
public:
    explicit Exception() {}
    explicit Exception(std::string messageToSet)
    {
        message = messageToSet;
    }
    virtual ~Exception() throw (){}
    virtual const char* GetMessage() const {return message.c_str();}
};

#endif // EXCEPTION_H
