#include "secure_logger_common_secure.h"
#include "traffic_light/Logger.edl.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static const char EntityName[] = "Logger";

/* Implementation of Write method. */
static nk_err_t WriteImpl(
                __rtl_unused struct Write         *self,
                const Write_WriteInLog_req        *req,
                const struct nk_arena             *reqArena,
                __rtl_unused Write_WriteInLog_res *res,
                __rtl_unused struct nk_arena      *resArena)
{
    FILE *fd = RTL_NULL;
    nk_uint32_t msgCount = 0;

    nk_ptr_t *messages = nk_arena_get(
                            nk_ptr_t, reqArena, &(req->message[0]), &msgCount);
    if (messages == RTL_NULL)
    {
        fprintf(
            stderr,
            "[%s]: Error: can`t get messages from arena!\n",
            EntityName);

        return NK_EBADMSG;
    }

    if ((fd = fopen(logPath, "a")))
    {
        setvbuf(fd, NULL, _IOLBF, 0);

        for (rtl_size_t i = 0; i < msgCount; i++)
        {
            nk_char_t *msg = RTL_NULL;
            nk_uint32_t msgLen = 0;

            msg = nk_arena_get(nk_char_t, reqArena, &messages[i], &msgLen);
            if (msg == RTL_NULL)
            {
                fprintf(
                    stderr,
                    "[%s]: Error: can`t get message from arena!\n",
                    EntityName);
                fclose(fd);

                return NK_EBADMSG;
            }

            if (msgLen > Write_WriteInLog_req_arena_size)
            {
                fprintf(
                    stderr,
                    "[%s]: Error: message length is bigger than arena size!\n",
                    EntityName);
                fclose(fd);

                return NK_EINVAL;
            }
            fprintf(fd, "%s\n", msg);
            fprintf(
                    stderr,
                    "[%s]: wrote to log: \"%s\"\n",
                    EntityName, msg);
        }

        fclose(fd);
    }
    else
    {
        fprintf(stderr, "[%s]: Error: can`t open log file!\n", EntityName);
    }

    return NK_EOK;
}

static struct Write *CreateIWriteImpl(void)
{
    /* Structure with implementation of Write interface methods. */
    static const struct Write_ops Ops =
    {
        .WriteInLog = WriteImpl
    };

    static Write obj =
    {
        .ops = &Ops
    };

    return &obj;
}

static Retcode CreateLogFile(void)
{
    int fdLog = -1;
    Retcode retCode = rcOk;
    mode_t modeLog = S_IRWXU | S_IRWXG | S_IRWXO;

    umask(S_IXUSR | S_IXGRP | S_IXOTH);

    if ((fdLog = open(logPath, O_WRONLY | O_CREAT, modeLog)) < 0)
    {
        fprintf(stderr, "[%s]: Error: can`t create log!\n", EntityName);
        retCode = rcFail;
    }

    close(fdLog);

    return retCode;
}

/* Entry point in Logger program. */
int main(void)
{
    ServiceId iid;
    NkKosTransport transport;

    fprintf(stderr, "[%s] started\n", EntityName);

    if (CreateLogFile() != rcOk)
    {
        fprintf(
            stderr,
            "[%s]: Error: can`t create log file! %s is terminating!\n",
            EntityName,
            EntityName);

        return EXIT_FAILURE;
    }

    fprintf(stderr, "[%s] log file created\n", EntityName);

    /**
     * Get server IPC-descriptor for connection with name
     * "logger_connection".
     */
    Handle handleClients = ServiceLocatorRegister(
                                        "logger_connection", NULL, 0, &iid);
    if (handleClients == INVALID_HANDLE)
    {
        fprintf(
            stderr,
            "[%s]: Error: can`t establish static IPC connection!\n",
            EntityName);

        return EXIT_FAILURE;
    }

    /* Initialize transport to clients. */
    NkKosTransport_Init(&transport, handleClients, NK_NULL, 0);

    /* Prepare response structures:fixed part and arena. */
    Logger_entity_req req;
    char reqBuffer[Logger_entity_req_arena_size];
    struct nk_arena reqArena = NK_ARENA_INITIALIZER(
                                reqBuffer, reqBuffer + sizeof(reqBuffer));

    /* Prepare response structures:fixed part. */
    Logger_entity_res res;

    /* Initialize Logger program dispatcher. */
    Logger_entity entity;
    Logger_entity_init(&entity, CreateIWriteImpl());

    fprintf(stderr, "[%s] IPC inited\n", EntityName);

    /* Main cycle: requests execution. */
    while (true)
    {
        /* Reset buffers with request and response. */
        nk_req_reset(&req);
        nk_arena_reset(&reqArena);

        if (nk_transport_recv(
                            &transport.base, &req.base_, &reqArena) == NK_EOK)
        {
            Logger_entity_dispatch(
                        &entity, &req.base_, &reqArena, &res.base_, RTL_NULL);
        }
        else
        {
            fprintf(stderr, "[%s]: nk_transport_recv error\n", EntityName);
        }

        if (nk_transport_reply(
                            &transport.base, &res.base_, RTL_NULL) != NK_EOK)
        {
            fprintf(stderr, "[%s]: nk_transport_reply error\n", EntityName);
        }
    }

    return EXIT_SUCCESS;
}
