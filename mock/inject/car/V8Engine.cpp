// Taken from [Dependency Injection in C++]
// (https://vladris.com/blog/2016/07/06/dependency-injection-in-c.html)

#if 0
#include "V8Engine.h"

void V8Engine::Start() {
    // start the engine
}

void V8Engine::Stop() {
    // stop the engine
}
#endif


#include "IEngine.h"

class V8Engine : public IEngine {
    void Start() override { /* start the engine */
    }
    void Stop() override { /* stop the engine */
    }
};

std::unique_ptr<IEngine> MakeV8Engine() { //
    return std::make_unique<V8Engine>();
}
