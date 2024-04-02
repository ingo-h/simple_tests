// Taken from [Dependency Injection in C++]
// (https://vladris.com/blog/2016/07/06/dependency-injection-in-c.html)

#if 0
#pragma once
#include "V8Engine.h"

class Car {
  public:
    void Drive();

  private:
    V8Engine m_engine;
};
#endif


#pragma once
#include "IEngine.h"

class Car {
  public:
    Car(std::unique_ptr<IEngine>&& engine) : m_engine(std::move(engine)) {}

    void Drive();

  private:
    std::unique_ptr<IEngine> m_engine;
};
