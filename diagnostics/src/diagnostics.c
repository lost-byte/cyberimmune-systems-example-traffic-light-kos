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
#include <traffic_light/IDiagComm.idl.h>

#include <assert.h>

#define EXAMPLE_VALUE_TO_SEND 777

static const char EntityName[] = "Diagnostics";



/* Соединение diagnostics->communication (исходящее) */
typedef struct{
    struct IDiagComm_proxy proxy;
    NkKosTransport transport;
    nk_iid_t riid;
    Handle handle;
    IDiagComm_DCode_req req;
    char req_buffer[IDiagnostics_DMessage_req_arena_size];
    struct nk_arena req_arena;
    IDiagComm_DCode_res res;
    //char res_buffer[IDiagnostics_DMessage_res_arena_size];
    //struct nk_arena res_arena;
}CDiagComm_TransportDescriptor;

CDiagComm_TransportDescriptor CDiagComm_td;

/* Инициализатор дескриптора diagnostics_connection */
void communication_connection_init(CDiagComm_TransportDescriptor *td){
    td->handle = ServiceLocatorConnect("diagcomm_connection");
    assert(td->handle != INVALID_HANDLE);
    NkKosTransport_Init(&td->transport, td->handle, NK_NULL, 0);
    td->riid = ServiceLocatorGetRiid(td->handle, "communication.dcode");
    assert(td->riid  != INVALID_RIID);
    IDiagComm_proxy_init(&td->proxy,
                                &td->transport.base,
                                td->riid);
    struct nk_arena arena = NK_ARENA_INITIALIZER(
                                td->req_buffer,
                                td->req_buffer + 
                                sizeof(td->req_buffer));
    td->req_arena = arena;
}

void communication_connection_send(CDiagComm_TransportDescriptor *td, 
                                    uint32_t code){
    nk_err_t rc;

    nk_req_reset(&td->req);
    //nk_arena_reset(&td->req_arena);

    td->req.code = code;

    if ((rc = IDiagComm_DCode(&td->proxy.base,
                            &td->req,
                            NULL,
                            &td->res,
                            NULL)
    ) != NK_EOK){
        fprintf(stderr, "[%s] IDiagComm_DCode error %d\n", EntityName, rc);
    }
}

/* структура - описатель IPC с LightsGPIO (входящее) */
typedef struct {
    NkKosTransport transport;
    ServiceId iid;
    Handle handle;
    Diagnostics_entity_req req;
    char req_buffer[Diagnostics_entity_req_arena_size];
    struct nk_arena req_arena;
    Diagnostics_entity_res res;
    char res_buffer[Diagnostics_entity_res_arena_size];
    struct nk_arena res_arena;
    Diagnostics_component component;
    Diagnostics_entity entity;
} CDmessage_TransportDescriptor;

/* Implementation of Dmessage method. */
static nk_err_t Dmesage(struct traffic_light_IDiagnostics *self,
                        const
                        struct traffic_light_IDiagnostics_DMessage_req *req,
                        const
                        struct nk_arena *req_arena,
                        struct traffic_light_IDiagnostics_DMessage_res *res,
                        struct nk_arena *res_arena)
{
    
    uint32_t code=req->code;

    nk_uint32_t msg_len = 0;    
    nk_ptr_t *msg = nk_arena_get(
                            nk_char_t, req_arena, &(req->msg), &msg_len);

    fprintf(stderr, "[%s]: Code[%u], Message:\"%s\"\n", EntityName, code, msg);

    

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

void diagnostics_connection_init(CDmessage_TransportDescriptor *td){

    /* Get lights gpio IPC handle of "control_system_connection". */
    td->handle = ServiceLocatorRegister("diagnostics_connection", NULL, 0, &td->iid);
    assert(td->handle != INVALID_HANDLE);

    /* Initialize transport to control system. */
    NkKosTransport_Init(&td->transport, td->handle, NK_NULL, 0);

    // Структуры запрос/ответ
    struct nk_arena req_arena = NK_ARENA_INITIALIZER(td->req_buffer,
                                        td->req_buffer + sizeof(td->req_buffer));

    
    struct nk_arena res_arena = NK_ARENA_INITIALIZER(td->res_buffer,
                                        td->res_buffer + sizeof(td->res_buffer));

    td->req_arena = req_arena;
    td->res_arena = res_arena;

    // диспетчер компонент
    CDmessage_component_init(&td->component, CreateIDiagnostics_Impl());

    /* Initialize lights gpio entity dispatcher. */
    Diagnostics_entity_init(&td->entity, &td->component);
}

/* Процессор  дескриптора control_system_connection  (входящее) */
void diagnostics_connection_loop(CDmessage_TransportDescriptor *td){
    /* Flush request/response buffers. */
    nk_req_reset(&td->req);
    nk_arena_reset(&td->req_arena);
    nk_arena_reset(&td->res_arena);

    /* Wait for request to lights gpio entity. */
    if (nk_transport_recv(&td->transport.base,
                            &td->req.base_,
                            &td->req_arena) != NK_EOK) {
        fprintf(stderr, "[%s] nk_transport_recv error\n", EntityName);
    } else {
        /**
         * Handle received request by calling implementation Mode_impl
         * of the requested Mode interface method.
         */
        Diagnostics_entity_dispatch(&td->entity, &td->req.base_, &td->req_arena,
                                    &td->res.base_, &td->res_arena);
    }

    /* Send response. */
    if (nk_transport_reply(&td->transport.base,
                            &td->res.base_,
                            &td->res_arena) != NK_EOK) {
        fprintf(stderr, "[%s] nk_transport_reply error\n", EntityName);
    }
}

/* Client entity entry point. */
int main(int argc, const char *argv[])
{
      
    CDmessage_TransportDescriptor CDmessage_td;
    
        
    fprintf(stderr, "[%s] started\n", EntityName);

    diagnostics_connection_init(&CDmessage_td);

    

    /* Рабочий цикл */
    while(true){
        diagnostics_connection_loop(&CDmessage_td);
        communication_connection_send(&CDiagComm_td, 12);
        
    };

    return EXIT_SUCCESS;
}
