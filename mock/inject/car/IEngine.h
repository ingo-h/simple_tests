// Taken from [Dependency Injection in C++]
// (https://vladris.com/blog/2016/07/06/dependency-injection-in-c.html)

#pragma once
#include <memory>

struct IEngine {
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual ~IEngine() = default;
};

std::unique_ptr<IEngine> MakeV8Engine();
