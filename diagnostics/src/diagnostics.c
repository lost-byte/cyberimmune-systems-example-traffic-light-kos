
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Files required for transport initialization. */
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>

#define NK_USE_UNQUALIFIED_NAMES

/* Description of the server interface used by the `client` entity. */
#include <traffic_light/Diagnostics.edl.h>

#include <assert.h>

#define EXAMPLE_VALUE_TO_SEND 777

static const char EntityName[] = "Diagnostics";

/* Implementation of Write method. */
static nk_err_t Dmesage(struct traffic_light_IDiagnostics *self,
                        const
                        struct traffic_light_IDiagnostics_DMessage_req *req,
                        const
                        struct nk_arena *req_arena,
                        struct traffic_light_IDiagnostics_DMessage_res *res,
                        struct nk_arena *res_arena)
{
    
    uint32_t code=req->code;

    fprintf(stderr, "[%s]: Dmesage got code=%u\n", EntityName, code);

    return NK_EOK;
}

static struct IDiagnostics *CreateIDiagnostics_Impl(void)
{
    /* Structure with implementation of Write interface methods. */
    static const struct IDiagnostics_ops Ops =
    {
        .DMessage = Dmesage
    };

    static struct IDiagnostics obj =
    {
        .ops = &Ops
    };

    return &obj;
}


/* Client entity entry point. */
int main(int argc, const char *argv[])
{
    
    NkKosTransport transport;
    ServiceId iid;
    
        
    fprintf(stderr, "[%s] started\n", EntityName);

    Handle handle = ServiceLocatorRegister("diagnostics_connection", NULL, 0, &iid);
    assert(handle != INVALID_HANDLE);

    NkKosTransport_Init(&transport, handle, NK_NULL, 0);

    /* Подготовка структур запроса и ответа */
    Diagnostics_entity_req req;
    Diagnostics_entity_res res;

    char req_buffer[Diagnostics_entity_req_arena_size];
    struct nk_arena req_arena = NK_ARENA_INITIALIZER(req_buffer,
                                        req_buffer + sizeof(req_buffer));

    char res_buffer[Diagnostics_entity_res_arena_size];
    struct nk_arena res_arena = NK_ARENA_INITIALIZER(res_buffer,
                                        res_buffer + sizeof(res_buffer));


    /* Инициализировать методы диспетчера компонента */
    CDmessage_component component;
    CDmessage_component_init(&component, CreateIDiagnostics_Impl());

    /* Инициализировать методы диспетчера сервера  */
    Diagnostics_entity entity;
    Diagnostics_entity_init(&entity, &component);

    /* Рабочий цикл */
    while(true){
        /* Flush request/response buffers. */
        nk_req_reset(&req);
        nk_arena_reset(&req_arena);
        nk_arena_reset(&res_arena);

        /* Wait for request to lights gpio entity. */
        if (nk_transport_recv(&transport.base,
                              &req.base_,
                              &req_arena) != NK_EOK) {
            fprintf(stderr, "nk_transport_recv error\n");
        } else {
            /**
             * Handle received request by calling implementation Mode_impl
             * of the requested Mode interface method.
             */
            Diagnostics_entity_dispatch(&entity, &req.base_, &req_arena,
                                        &res.base_, &res_arena);
        }

        /* Send response. */
        if (nk_transport_reply(&transport.base,
                               &res.base_,
                               &res_arena) != NK_EOK) {
            fprintf(stderr, "nk_transport_reply error\n");
        }
    };

    return EXIT_SUCCESS;
}
