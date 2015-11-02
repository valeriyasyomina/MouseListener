#include "Singleton.h"

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
    if (!facade)
        facade = new Facade();
    return facade;
}


