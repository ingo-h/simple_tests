// Taken from [Dependency Injection in C++]
// (https://vladris.com/blog/2016/07/06/dependency-injection-in-c.html)

#include "Car.h"
#include <iostream>

class MockEngine : public IEngine {
    void Start() override { /* mock logic */
	std::cout << "Start mocked IEngine\n";
    }
    void Stop() override { /* mock logic */
	std::cout << "Stop mocked IEngine\n";
    }
};

void Test() {
    Car car(std::make_unique<MockEngine>());

    // Test Car without a real Engine
    car.Drive();
}

int main() {
    Test();
    return 0;
}
