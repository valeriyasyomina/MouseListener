#ifndef SINGLETON_H
#define SINGLETON_H

#include "Facade/Facade.h"

class Singleton
{
private:
    Singleton();
    ~Singleton();
public:
    static Singleton& Instance() {return instance;}
    Facade* GetFacade();
private:
    static Singleton instance;
    Facade* facade;
};

#endif // SINGLETON_H
