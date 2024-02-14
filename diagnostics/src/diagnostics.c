
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Files required for transport initialization. */
#include <coresrv/nk/transport-kos.h>

#define NK_USE_UNQUALIFIED_NAMES

/* Description of the server interface used by the `client` entity. */
#include <traffic_light/IDiagnostics.idl.h>

#include <assert.h>

#define EXAMPLE_VALUE_TO_SEND 777

static const char EntityName[] = "Diagnostics";

/* Client entity entry point. */
int main(int argc, const char *argv[])
{
    /*
    NkKosTransport transport;
    struct echo_Ping_proxy proxy;
    int i;
    */
    fprintf(stderr, "[%s]] started\n", EntityName);

    

    while(true){};

    return EXIT_SUCCESS;
}
