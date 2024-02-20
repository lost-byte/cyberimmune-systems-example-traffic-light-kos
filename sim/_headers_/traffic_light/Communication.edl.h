#pragma GCC diagnostic push
#include <nk/transport.h>
#include <nk/types.h>
#ifndef __traffic_light__Communication__COMPONENT_ENDPOINTS__
#define __traffic_light__Communication__COMPONENT_ENDPOINTS__
enum {
    traffic_light_Communication_iidMax,
};
enum {
    traffic_light_Communication_iidOffset = 0,
};
enum {
    traffic_light_Communication_securityIidMax,
};
enum {
    traffic_light_Communication_component_req_arena_size =
    0,
    traffic_light_Communication_component_res_arena_size =
    0,
    traffic_light_Communication_component_arena_size =
    0,
    traffic_light_Communication_component_req_handles =
    0,
    traffic_light_Communication_component_res_handles =
    0,
    traffic_light_Communication_component_err_handles =
    0,
};
#ifdef NK_USE_UNQUALIFIED_NAMES
#define Communication_iidMax traffic_light_Communication_iidMax
#define Communication_iidOffset traffic_light_Communication_iidOffset
#define Communication_securityIidMax traffic_light_Communication_securityIidMax
#define Communication_component_req_arena_size traffic_light_Communication_component_req_arena_size
#define Communication_component_res_arena_size traffic_light_Communication_component_res_arena_size
#define Communication_component_arena_size traffic_light_Communication_component_arena_size
#define Communication_component_req_handles traffic_light_Communication_component_req_handles
#define Communication_component_res_handles traffic_light_Communication_component_res_handles
#define Communication_component_err_handles traffic_light_Communication_component_err_handles
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __traffic_light__Communication__COMPONENT_ENDPOINTS__ */

#ifndef __traffic_light__Communication__TASK_ENDPOINTS__
#define __traffic_light__Communication__TASK_ENDPOINTS__
enum {
    traffic_light_Communication_entity_req_arena_size =
    traffic_light_Communication_component_req_arena_size,
    traffic_light_Communication_entity_res_arena_size =
    traffic_light_Communication_component_res_arena_size,
    traffic_light_Communication_entity_arena_size =
    traffic_light_Communication_component_arena_size,
    traffic_light_Communication_entity_req_handles =
    traffic_light_Communication_component_req_handles,
    traffic_light_Communication_entity_res_handles =
    traffic_light_Communication_component_res_handles,
    traffic_light_Communication_entity_err_handles =
    traffic_light_Communication_component_err_handles,
};
#ifdef NK_USE_UNQUALIFIED_NAMES
#define Communication_entity_req_arena_size traffic_light_Communication_entity_req_arena_size
#define Communication_entity_res_arena_size traffic_light_Communication_entity_res_arena_size
#define Communication_entity_arena_size traffic_light_Communication_entity_arena_size
#define Communication_entity_req_handles traffic_light_Communication_entity_req_handles
#define Communication_entity_res_handles traffic_light_Communication_entity_res_handles
#define Communication_entity_err_handles traffic_light_Communication_entity_err_handles
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __traffic_light__Communication__TASK_ENDPOINTS__ */

#pragma GCC diagnostic pop

