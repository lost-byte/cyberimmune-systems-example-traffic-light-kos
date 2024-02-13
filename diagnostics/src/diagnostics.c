
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Files required for transport initialization. */
//#include <coresrv/nk/transport-kos.h>
//#include <coresrv/sl/sl_api.h>

/* Description of the server interface used by the `client` entity. */
//#include <echo/Ping.idl.h>

#include <assert.h>

#define EXAMPLE_VALUE_TO_SEND 777

/* Client entity entry point. */
int main(int argc, const char *argv[])
{
    /*
    NkKosTransport transport;
    struct echo_Ping_proxy proxy;
    int i;
    */

    fprintf(stderr, "Diagnostics started\n");

    while(true){};

    return EXIT_SUCCESS;
}
