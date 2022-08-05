#include <stdio.h>
#include <upnp/upnp.h>
#include <upnp/upnptools.h>   // UpnpGetErrorMessage
#include <upnp/upnpdebug.h>   // UpnpSetLogFileNames

int main()
{
    constexpr char INTERFACE[] = "ens1";
    unsigned short PORT = 51515;

    constexpr char ErrFileName[]  = "/dev/stderr";
    constexpr char InfoFileName[] = "/dev/stdout";
    int rc;

    UpnpSetLogFileNames(ErrFileName, InfoFileName);

    rc = UpnpInit2(INTERFACE, PORT);
    printf("UpnpInit2(interface=%s, port=%d) return code: %s (%d)\n",
           INTERFACE, PORT, UpnpGetErrorMessage(rc), rc);
}
