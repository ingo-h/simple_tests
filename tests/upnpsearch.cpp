#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <upnp/upnp.h>

constexpr char INTERFACE[] = "ens1";
constexpr int SEARCH_DURATION_SECONDS = 5;


int callback_event_handler(Upnp_EventType event_type, void* event, void* cookie)
{
    std::string str;

    switch (event_type) {
        case UPNP_DISCOVERY_SEARCH_RESULT:
            str = "UPNP_DISCOVERY_SEARCH_RESULT";
            break;
        case UPNP_DISCOVERY_SEARCH_TIMEOUT:
            str = "UPNP_DISCOVERY_SEARCH_TIMEOUT";
            break;
        default:
            std::cout << "Called callback_event_handler with ignored event" << std::endl;
            return 0;
    }
    std::cout << "Executing callback_event_handler with event type " << str << std::endl;
    std::cout << (char*)cookie << std::endl;
    return 0;
}


int main()
{
    constexpr char cookie[] = "Free custom data, usable in the callback function.";
    UpnpClient_Handle client_handle = -1;

    UpnpInit2(INTERFACE, 0);
    UpnpRegisterClient(callback_event_handler, &cookie, &client_handle);
    UpnpSearchAsync(client_handle, SEARCH_DURATION_SECONDS, "upnp:rootdevice", cookie);
    std::this_thread::sleep_for(std::chrono::seconds(SEARCH_DURATION_SECONDS + 1));
    UpnpFinish();
}
