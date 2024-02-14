#pragma GCC diagnostic push
#include <nk/transport.h>
#include <nk/types.h>
#ifndef ____UInt32__RAWTYPE__
#define ____UInt32__RAWTYPE__
nk_static_assert(sizeof(nk_uint32_t) == 4, bad_nk_uint32_t_size);
nk_static_assert(nk_alignof(nk_uint32_t) == 4, bad_nk_uint32_t_align);
#endif /* ____UInt32__RAWTYPE__ */

#ifndef __traffic_light__IDiagnostics__INTERFACE__
#define __traffic_light__IDiagnostics__INTERFACE__
enum {
    traffic_light_IDiagnostics_DMessage_mid,
    traffic_light_IDiagnostics_mid_max,
};
enum {
    traffic_light_IDiagnostics_DMessage_req_arena_size =
    0,
    traffic_light_IDiagnostics_DMessage_res_arena_size =
    0,
    traffic_light_IDiagnostics_DMessage_req_handles =
    0,
    traffic_light_IDiagnostics_DMessage_res_handles =
    0,
    traffic_light_IDiagnostics_DMessage_err_handles =
    0,
    traffic_light_IDiagnostics_req_arena_size = 0,
    traffic_light_IDiagnostics_res_arena_size = 0,
    traffic_light_IDiagnostics_arena_size = 0,
    traffic_light_IDiagnostics_req_handles = 0,
    traffic_light_IDiagnostics_res_handles = 0,
    traffic_light_IDiagnostics_err_handles = 0,
};
typedef struct __nk_packed traffic_light_IDiagnostics_DMessage_req {
            __nk_alignas(8)
            struct nk_message base_;
            __nk_alignas(4) nk_uint32_t code;
        } traffic_light_IDiagnostics_DMessage_req;
#pragma pack(push, 8) /* traffic_light_IDiagnostics_DMessage_res */
typedef struct traffic_light_IDiagnostics_DMessage_err {
            __nk_alignas(8)
            struct nk_message base_;
        } traffic_light_IDiagnostics_DMessage_err;
typedef struct traffic_light_IDiagnostics_DMessage_res {
            union {
                struct {
                    __nk_alignas(8)
                    struct nk_message base_;
                };
                struct {
                    __nk_alignas(8)
                    struct nk_message base_;
                } res_;
                struct traffic_light_IDiagnostics_DMessage_err err_;
            };
        } traffic_light_IDiagnostics_DMessage_res;
#pragma pack(pop) /* traffic_light_IDiagnostics_DMessage_res */
#ifdef NK_USE_UNQUALIFIED_NAMES
#define IDiagnostics_DMessage_mid traffic_light_IDiagnostics_DMessage_mid
#define IDiagnostics_mid_max traffic_light_IDiagnostics_mid_max
#define IDiagnostics_DMessage_req_arena_size traffic_light_IDiagnostics_DMessage_req_arena_size
#define IDiagnostics_DMessage_res_arena_size traffic_light_IDiagnostics_DMessage_res_arena_size
#define IDiagnostics_DMessage_req_handles traffic_light_IDiagnostics_DMessage_req_handles
#define IDiagnostics_DMessage_res_handles traffic_light_IDiagnostics_DMessage_res_handles
#define IDiagnostics_DMessage_err_handles traffic_light_IDiagnostics_DMessage_err_handles
#define IDiagnostics_req_arena_size traffic_light_IDiagnostics_req_arena_size
#define IDiagnostics_res_arena_size traffic_light_IDiagnostics_res_arena_size
#define IDiagnostics_arena_size traffic_light_IDiagnostics_arena_size
#define IDiagnostics_req_handles traffic_light_IDiagnostics_req_handles
#define IDiagnostics_res_handles traffic_light_IDiagnostics_res_handles
#define IDiagnostics_err_handles traffic_light_IDiagnostics_err_handles
#define IDiagnostics_DMessage_req traffic_light_IDiagnostics_DMessage_req
#define IDiagnostics_DMessage_res traffic_light_IDiagnostics_DMessage_res
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __traffic_light__IDiagnostics__INTERFACE__ */

#ifndef __traffic_light__IDiagnostics__VTABLE__
#define __traffic_light__IDiagnostics__VTABLE__
typedef struct traffic_light_IDiagnostics {
            const
            struct traffic_light_IDiagnostics_ops *ops;
        } traffic_light_IDiagnostics;
typedef nk_err_t
traffic_light_IDiagnostics_DMessage_fn(struct traffic_light_IDiagnostics *,
                                       const
                                       struct traffic_light_IDiagnostics_DMessage_req *,
                                       const
                                       struct nk_arena *,
                                       struct traffic_light_IDiagnostics_DMessage_res *,
                                       struct nk_arena *);
typedef struct traffic_light_IDiagnostics_ops {
            traffic_light_IDiagnostics_DMessage_fn *DMessage;
        } traffic_light_IDiagnostics_ops;
#ifdef NK_USE_UNQUALIFIED_NAMES
#define IDiagnostics_DMessage_fn traffic_light_IDiagnostics_DMessage_fn
#define IDiagnostics_ops traffic_light_IDiagnostics_ops
#define IDiagnostics traffic_light_IDiagnostics
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __traffic_light__IDiagnostics__VTABLE__ */

#ifndef __traffic_light__IDiagnostics__CLIENT__
#define __traffic_light__IDiagnostics__CLIENT__
typedef struct traffic_light_IDiagnostics_proxy {
            struct traffic_light_IDiagnostics base;
            struct nk_transport *transport;
            nk_iid_t iid;
        } traffic_light_IDiagnostics_proxy;
static inline
nk_err_t traffic_light_IDiagnostics_DMessage_proxy(struct traffic_light_IDiagnostics_proxy *self,
                                                   struct traffic_light_IDiagnostics_DMessage_req *req,
                                                   const
                                                   struct nk_arena *req_arena,
                                                   struct traffic_light_IDiagnostics_DMessage_res *res,
                                                   struct nk_arena *res_arena)
{
    nk_err_t rc;
    
    nk_assert(self != NK_NULL);
    nk_assert(req != NK_NULL);
    nk_assert(res != NK_NULL);
    nk_req_reset(req);
    nk_msg_set_method_id(req, self->iid,
                         traffic_light_IDiagnostics_DMessage_mid);
    nk_msg_set_ncaps(req,
                     traffic_light_IDiagnostics_DMessage_req_handles);
    nk_req_reset(res);
    nk_msg_set_method_id(res, self->iid,
                         traffic_light_IDiagnostics_DMessage_mid);
    nk_msg_set_ncaps(res,
                     traffic_light_IDiagnostics_DMessage_res_handles);
    rc = nk_transport_call(self->transport,
                           &req->base_, req_arena,
                           &res->base_,
                           res_arena);
    return rc;
}
static inline
void traffic_light_IDiagnostics_proxy_init(struct traffic_light_IDiagnostics_proxy *self,
                                           struct nk_transport *transport,
                                           nk_iid_t iid)
{
    static const
    struct traffic_light_IDiagnostics_ops ops =
                                          {(traffic_light_IDiagnostics_DMessage_fn *) &traffic_light_IDiagnostics_DMessage_proxy};
    
    nk_assert(self != NK_NULL);
    self->base.ops = &ops;
    self->transport = transport;
    self->iid = iid;
}
static inline
nk_err_t traffic_light_IDiagnostics_DMessage(struct traffic_light_IDiagnostics *self,
                                             struct traffic_light_IDiagnostics_DMessage_req *req,
                                             const
                                             struct nk_arena *req_arena,
                                             struct traffic_light_IDiagnostics_DMessage_res *res,
                                             struct nk_arena *res_arena)
{
    nk_assert(self != NK_NULL);
    nk_assert(self->ops != NK_NULL);
    nk_assert(self->ops->DMessage != NK_NULL);
    return self->ops->DMessage(self, req,
                               req_arena, res,
                               res_arena);
}
#ifdef NK_USE_UNQUALIFIED_NAMES
#define IDiagnostics_DMessage_proxy traffic_light_IDiagnostics_DMessage_proxy
#define IDiagnostics_DMessage traffic_light_IDiagnostics_DMessage
#define IDiagnostics_proxy traffic_light_IDiagnostics_proxy
#define IDiagnostics_proxy_init traffic_light_IDiagnostics_proxy_init
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __traffic_light__IDiagnostics__CLIENT__ */

#ifndef __traffic_light__IDiagnostics__SERVER__
#define __traffic_light__IDiagnostics__SERVER__
typedef union traffic_light_IDiagnostics_req {
            struct nk_message base_;
            struct traffic_light_IDiagnostics_DMessage_req DMessage;
        } traffic_light_IDiagnostics_req;
typedef union traffic_light_IDiagnostics_res {
            struct nk_message base_;
            struct traffic_light_IDiagnostics_DMessage_res DMessage;
        } traffic_light_IDiagnostics_res;
static inline
nk_err_t traffic_light_IDiagnostics_interface_dispatch(struct traffic_light_IDiagnostics *impl,
                                                       nk_iid_t iid,
                                                       const
                                                       struct nk_message *req,
                                                       const
                                                       struct nk_arena *req_arena,
                                                       struct nk_message *res,
                                                       struct nk_arena *res_arena)
{
    nk_err_t rc = NK_ENOENT;
    nk_iid_t req_iid;
    nk_mid_t req_mid;
    
    nk_assert(impl != NK_NULL);
    nk_assert(req != NK_NULL);
    nk_assert(res != NK_NULL);
    nk_unused(impl);
    nk_unused(req_arena);
    nk_unused(res_arena);
    if (req->iid == iid) {
        req_iid = req->iid;
        req_mid = req->mid;
        switch (req->mid) {
            
          case traffic_light_IDiagnostics_DMessage_mid:
            {
                struct traffic_light_IDiagnostics_DMessage_res
                *res_ =
                (struct traffic_light_IDiagnostics_DMessage_res *) res;
                
                rc = impl->ops->DMessage(impl,
                                         (const
                                          struct traffic_light_IDiagnostics_DMessage_req *) req,
                                         req_arena,
                                         res_,
                                         res_arena);
                if (rc == NK_EOK) {
                    nk_req_reset(&res_->res_);
                    nk_msg_set_ncaps(res_,
                                     traffic_light_IDiagnostics_DMessage_res_handles);
                }
                break;
            }
            
          default:
            ;
        }
        res->iid = req_iid;
        res->mid = req_mid;
    }
    return rc;
}
#ifdef NK_USE_UNQUALIFIED_NAMES
#define IDiagnostics_req traffic_light_IDiagnostics_req
#define IDiagnostics_res traffic_light_IDiagnostics_res
#define IDiagnostics_interface_dispatch traffic_light_IDiagnostics_interface_dispatch
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __traffic_light__IDiagnostics__SERVER__ */

#ifndef __traffic_light__IDiagnostics__TESTS__
#define __traffic_light__IDiagnostics__TESTS__
static inline
nk_err_t traffic_light_IDiagnostics_DMessage_test_req(struct traffic_light_IDiagnostics_DMessage_req *msg,
                                                      struct nk_arena *arena,
                                                      int (*rand)(void),
                                                      nk_bool_t *failure)
{
    nk_unused(msg);
    nk_unused(arena);
    nk_unused(rand);
    *failure = NK_FALSE;
    msg->code = (nk_uint32_t) rand();
    return NK_EOK;
}
static inline
nk_err_t traffic_light_IDiagnostics_DMessage_test_res(struct traffic_light_IDiagnostics_DMessage_res *msg,
                                                      struct nk_arena *arena,
                                                      int (*rand)(void),
                                                      nk_bool_t *failure)
{
    nk_unused(msg);
    nk_unused(arena);
    nk_unused(rand);
    *failure = NK_FALSE;
    return NK_EOK;
}
static inline
nk_err_t traffic_light_IDiagnostics_DMessage_test_err(struct traffic_light_IDiagnostics_DMessage_err *msg,
                                                      struct nk_arena *arena,
                                                      int (*rand)(void),
                                                      nk_bool_t *failure)
{
    nk_unused(msg);
    nk_unused(arena);
    nk_unused(rand);
    *failure = NK_FALSE;
    return NK_EOK;
}
#endif /* __traffic_light__IDiagnostics__TESTS__ */

#pragma GCC diagnostic pop

