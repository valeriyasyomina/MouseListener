#include "Singleton.h"
#include "Exception/AllocMemoryException.h"

Singleton Singleton::instance;

Singleton::Singleton()
{
    facade = new Facade();
    qDebug()<< "Singlon ctor";
}

Singleton::~Singleton()
{
    if (facade)
    {
        delete facade;
        facade = NULL;
    }
}

Facade* Singleton::GetFacade()
{
    try
    {
        if (!facade)
            facade = new Facade();
        return facade;
    }
    catch (std::bad_alloc& exception)
    {
        throw AllocMemoryException("Error allocate memory in Singleton::GetFacade()");
    }
}


