// Taken from [Dependency Injection in C++]
// (https://vladris.com/blog/2016/07/06/dependency-injection-in-c.html)

#if 0
#include "Car.h"

void Car::Drive() {
    m_engine.Start();
    // drive
    m_engine.Stop();
}
#endif


#include "Car.h"

void Car::Drive() {
    m_engine->Start();
    // drive
    m_engine->Stop();
}
