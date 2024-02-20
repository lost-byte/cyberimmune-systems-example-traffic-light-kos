#pragma GCC diagnostic push
#include <nk/transport.h>
#include <nk/types.h>
#ifndef ____UInt8__RAWTYPE__
#define ____UInt8__RAWTYPE__
nk_static_assert(sizeof(nk_uint8_t) == 1, bad_nk_uint8_t_size);
nk_static_assert(nk_alignof(nk_uint8_t) == 1, bad_nk_uint8_t_align);
#endif /* ____UInt8__RAWTYPE__ */

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
    traffic_light_IDiagnostics_DMessage_req_msg_size =
    256,
    traffic_light_IDiagnostics_DMessage_req_arena_size =
    256,
    traffic_light_IDiagnostics_DMessage_res_arena_size =
    0,
    traffic_light_IDiagnostics_DMessage_req_handles =
    0,
    traffic_light_IDiagnostics_DMessage_res_handles =
    0,
    traffic_light_IDiagnostics_DMessage_err_handles =
    0,
    traffic_light_IDiagnostics_req_arena_size =
    256,
    traffic_light_IDiagnostics_res_arena_size = 0,
    traffic_light_IDiagnostics_arena_size = 256,
    traffic_light_IDiagnostics_req_handles = 0,
    traffic_light_IDiagnostics_res_handles = 0,
    traffic_light_IDiagnostics_err_handles = 0,
};
typedef struct __nk_packed traffic_light_IDiagnostics_DMessage_req {
            __nk_alignas(8)
            struct nk_message base_;
            __nk_alignas(4) nk_uint32_t code;
            __nk_alignas(4) nk_ptr_t msg;
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
#define IDiagnostics_DMessage_req_msg_size traffic_light_IDiagnostics_DMessage_req_msg_size
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

#ifndef __traffic_light__CDmessage__COMPONENT_ENDPOINTS__
#define __traffic_light__CDmessage__COMPONENT_ENDPOINTS__
enum {
    traffic_light_CDmessage_dmessage_iid,
    traffic_light_CDmessage_iidMax,
};
enum {
    traffic_light_CDmessage_iidOffset = 0,
};
enum {
    traffic_light_CDmessage_securityIidMax,
};
enum {
    traffic_light_CDmessage_component_req_arena_size =
    256,
    traffic_light_CDmessage_component_res_arena_size =
    0,
    traffic_light_CDmessage_component_arena_size =
    256,
    traffic_light_CDmessage_component_req_handles =
    0,
    traffic_light_CDmessage_component_res_handles =
    0,
    traffic_light_CDmessage_component_err_handles =
    0,
};
#ifdef NK_USE_UNQUALIFIED_NAMES
#define CDmessage_dmessage_iid traffic_light_CDmessage_dmessage_iid
#define CDmessage_iidMax traffic_light_CDmessage_iidMax
#define CDmessage_iidOffset traffic_light_CDmessage_iidOffset
#define CDmessage_securityIidMax traffic_light_CDmessage_securityIidMax
#define CDmessage_component_req_arena_size traffic_light_CDmessage_component_req_arena_size
#define CDmessage_component_res_arena_size traffic_light_CDmessage_component_res_arena_size
#define CDmessage_component_arena_size traffic_light_CDmessage_component_arena_size
#define CDmessage_component_req_handles traffic_light_CDmessage_component_req_handles
#define CDmessage_component_res_handles traffic_light_CDmessage_component_res_handles
#define CDmessage_component_err_handles traffic_light_CDmessage_component_err_handles
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __traffic_light__CDmessage__COMPONENT_ENDPOINTS__ */

#ifndef __traffic_light__Diagnostics__COMPONENT_ENDPOINTS__
#define __traffic_light__Diagnostics__COMPONENT_ENDPOINTS__
enum {
    traffic_light_Diagnostics_diagnostics_dmessage_iid,
    traffic_light_Diagnostics_iidMax,
};
enum {
    traffic_light_Diagnostics_iidOffset = 0,
    traffic_light_Diagnostics_diagnostics_iidOffset =
    0,
};
enum {
    traffic_light_Diagnostics_securityIidMax,
};
enum {
    traffic_light_Diagnostics_component_req_arena_size =
    256,
    traffic_light_Diagnostics_component_res_arena_size =
    0,
    traffic_light_Diagnostics_component_arena_size =
    256,
    traffic_light_Diagnostics_component_req_handles =
    0,
    traffic_light_Diagnostics_component_res_handles =
    0,
    traffic_light_Diagnostics_component_err_handles =
    0,
};
#ifdef NK_USE_UNQUALIFIED_NAMES
#define Diagnostics_diagnostics_dmessage_iid traffic_light_Diagnostics_diagnostics_dmessage_iid
#define Diagnostics_iidMax traffic_light_Diagnostics_iidMax
#define Diagnostics_iidOffset traffic_light_Diagnostics_iidOffset
#define Diagnostics_diagnostics_iidOffset traffic_light_Diagnostics_diagnostics_iidOffset
#define Diagnostics_securityIidMax traffic_light_Diagnostics_securityIidMax
#define Diagnostics_component_req_arena_size traffic_light_Diagnostics_component_req_arena_size
#define Diagnostics_component_res_arena_size traffic_light_Diagnostics_component_res_arena_size
#define Diagnostics_component_arena_size traffic_light_Diagnostics_component_arena_size
#define Diagnostics_component_req_handles traffic_light_Diagnostics_component_req_handles
#define Diagnostics_component_res_handles traffic_light_Diagnostics_component_res_handles
#define Diagnostics_component_err_handles traffic_light_Diagnostics_component_err_handles
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __traffic_light__Diagnostics__COMPONENT_ENDPOINTS__ */

#ifndef __traffic_light__Diagnostics__TASK_ENDPOINTS__
#define __traffic_light__Diagnostics__TASK_ENDPOINTS__
enum {
    traffic_light_Diagnostics_entity_req_arena_size =
    traffic_light_Diagnostics_component_req_arena_size,
    traffic_light_Diagnostics_entity_res_arena_size =
    traffic_light_Diagnostics_component_res_arena_size,
    traffic_light_Diagnostics_entity_arena_size =
    traffic_light_Diagnostics_component_arena_size,
    traffic_light_Diagnostics_entity_req_handles =
    traffic_light_Diagnostics_component_req_handles,
    traffic_light_Diagnostics_entity_res_handles =
    traffic_light_Diagnostics_component_res_handles,
    traffic_light_Diagnostics_entity_err_handles =
    traffic_light_Diagnostics_component_err_handles,
};
#ifdef NK_USE_UNQUALIFIED_NAMES
#define Diagnostics_entity_req_arena_size traffic_light_Diagnostics_entity_req_arena_size
#define Diagnostics_entity_res_arena_size traffic_light_Diagnostics_entity_res_arena_size
#define Diagnostics_entity_arena_size traffic_light_Diagnostics_entity_arena_size
#define Diagnostics_entity_req_handles traffic_light_Diagnostics_entity_req_handles
#define Diagnostics_entity_res_handles traffic_light_Diagnostics_entity_res_handles
#define Diagnostics_entity_err_handles traffic_light_Diagnostics_entity_err_handles
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __traffic_light__Diagnostics__TASK_ENDPOINTS__ */

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

#ifndef __traffic_light__CDmessage__COMPONENT_SERVER_TYPES__
#define __traffic_light__CDmessage__COMPONENT_SERVER_TYPES__
typedef struct traffic_light_CDmessage_component {
            struct traffic_light_IDiagnostics *dmessage;
        } traffic_light_CDmessage_component;
#endif /* __traffic_light__CDmessage__COMPONENT_SERVER_TYPES__ */

#ifndef __traffic_light__CDmessage__COMPONENT_SERVER_DISPATCHER__
#define __traffic_light__CDmessage__COMPONENT_SERVER_DISPATCHER__
static inline
void traffic_light_CDmessage_component_init(struct traffic_light_CDmessage_component *self,
                                            struct traffic_light_IDiagnostics *dmessage)
{
    nk_assert(self != NK_NULL);
    nk_unused(self);
    self->dmessage = dmessage;
}
typedef union traffic_light_CDmessage_component_req {
            struct nk_message base_;
            traffic_light_IDiagnostics_req dmessage;
        } traffic_light_CDmessage_component_req;
typedef union traffic_light_CDmessage_component_res {
            struct nk_message base_;
            traffic_light_IDiagnostics_res dmessage;
        } traffic_light_CDmessage_component_res;
static inline
nk_err_t traffic_light_CDmessage_component_dispatch(struct traffic_light_CDmessage_component *self,
                                                    nk_iid_t iidOffset,
                                                    const
                                                    struct nk_message *req,
                                                    const
                                                    struct nk_arena *req_arena,
                                                    struct nk_message *res,
                                                    struct nk_arena *res_arena)
{
    nk_assert(self != NK_NULL);
    nk_assert(req != NK_NULL);
    nk_assert(res != NK_NULL);
    nk_unused(self);
    nk_unused(req_arena);
    nk_unused(res);
    nk_unused(res_arena);
    if (req->iid < iidOffset)
        return NK_ENOENT;
    switch (req->iid - iidOffset) {
        
      case traffic_light_CDmessage_dmessage_iid:
        return traffic_light_IDiagnostics_interface_dispatch(self->dmessage,
                                                             req->iid,
                                                             req,
                                                             req_arena,
                                                             res,
                                                             res_arena);
        
      default:
        ;
    }
    return NK_ENOENT;
}
#ifdef NK_USE_UNQUALIFIED_NAMES
#define CDmessage_component traffic_light_CDmessage_component
#define CDmessage_component_init traffic_light_CDmessage_component_init
#define CDmessage_component_req traffic_light_CDmessage_component_req
#define CDmessage_component_res traffic_light_CDmessage_component_res
#define CDmessage_component_dispatch traffic_light_CDmessage_component_dispatch
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __traffic_light__CDmessage__COMPONENT_SERVER_DISPATCHER__ */

#ifndef __traffic_light__Diagnostics__COMPONENT_SERVER_TYPES__
#define __traffic_light__Diagnostics__COMPONENT_SERVER_TYPES__
typedef struct traffic_light_Diagnostics_component {
            struct traffic_light_CDmessage_component *diagnostics;
        } traffic_light_Diagnostics_component;
#endif /* __traffic_light__Diagnostics__COMPONENT_SERVER_TYPES__ */

#ifndef __traffic_light__Diagnostics__COMPONENT_SERVER_DISPATCHER__
#define __traffic_light__Diagnostics__COMPONENT_SERVER_DISPATCHER__
static inline
void traffic_light_Diagnostics_component_init(struct traffic_light_Diagnostics_component *self,
                                              struct traffic_light_CDmessage_component *diagnostics)
{
    nk_assert(self != NK_NULL);
    nk_unused(self);
    self->diagnostics = diagnostics;
}
typedef union traffic_light_Diagnostics_component_req {
            struct nk_message base_;
            traffic_light_IDiagnostics_req diagnostics_dmessage;
        } traffic_light_Diagnostics_component_req;
typedef union traffic_light_Diagnostics_component_res {
            struct nk_message base_;
            traffic_light_IDiagnostics_res diagnostics_dmessage;
        } traffic_light_Diagnostics_component_res;
static inline
nk_err_t traffic_light_Diagnostics_component_dispatch(struct traffic_light_Diagnostics_component *self,
                                                      nk_iid_t iidOffset,
                                                      const
                                                      struct nk_message *req,
                                                      const
                                                      struct nk_arena *req_arena,
                                                      struct nk_message *res,
                                                      struct nk_arena *res_arena)
{
    nk_assert(self != NK_NULL);
    nk_assert(req != NK_NULL);
    nk_assert(res != NK_NULL);
    nk_unused(self);
    nk_unused(req_arena);
    nk_unused(res);
    nk_unused(res_arena);
    if (req->iid < iidOffset)
        return NK_ENOENT;
    switch (req->iid - iidOffset) {
        
      case traffic_light_Diagnostics_diagnostics_dmessage_iid:
        return traffic_light_IDiagnostics_interface_dispatch(self->diagnostics->dmessage,
                                                             req->iid,
                                                             req,
                                                             req_arena,
                                                             res,
                                                             res_arena);
        
      default:
        ;
    }
    return NK_ENOENT;
}
#ifdef NK_USE_UNQUALIFIED_NAMES
#define Diagnostics_component traffic_light_Diagnostics_component
#define Diagnostics_component_init traffic_light_Diagnostics_component_init
#define Diagnostics_component_req traffic_light_Diagnostics_component_req
#define Diagnostics_component_res traffic_light_Diagnostics_component_res
#define Diagnostics_component_dispatch traffic_light_Diagnostics_component_dispatch
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __traffic_light__Diagnostics__COMPONENT_SERVER_DISPATCHER__ */

#ifndef __traffic_light__Diagnostics__ENTITY_SERVER__
#define __traffic_light__Diagnostics__ENTITY_SERVER__
#define traffic_light_Diagnostics_entity_res traffic_light_Diagnostics_component_res
#define traffic_light_Diagnostics_entity_req traffic_light_Diagnostics_component_req
#define traffic_light_Diagnostics_entity traffic_light_Diagnostics_component
static inline
void traffic_light_Diagnostics_entity_init(struct traffic_light_Diagnostics_entity *self,
                                           struct traffic_light_CDmessage_component *diagnostics)
{
    traffic_light_Diagnostics_component_init(self,
                                             diagnostics);
}
static inline
nk_err_t traffic_light_Diagnostics_entity_dispatch(struct traffic_light_Diagnostics_entity *self,
                                                   const
                                                   struct nk_message *req,
                                                   const
                                                   struct nk_arena *req_arena,
                                                   struct nk_message *res,
                                                   struct nk_arena *res_arena)
{
    return traffic_light_Diagnostics_component_dispatch(self,
                                                        0,
                                                        req,
                                                        req_arena,
                                                        res,
                                                        res_arena);
}
#ifdef NK_USE_UNQUALIFIED_NAMES
#define Diagnostics_entity traffic_light_Diagnostics_entity
#define Diagnostics_entity_init traffic_light_Diagnostics_entity_init
#define Diagnostics_entity_req traffic_light_Diagnostics_entity_req
#define Diagnostics_entity_res traffic_light_Diagnostics_entity_res
#define Diagnostics_entity_dispatch traffic_light_Diagnostics_entity_dispatch
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __traffic_light__Diagnostics__ENTITY_SERVER__ */

#pragma GCC diagnostic pop

