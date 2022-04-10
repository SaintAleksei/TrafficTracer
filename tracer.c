#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dr_api.h"
#include "drmgr.h"
#include "drwrap.h"
#include "hashtable.h"
#include "wrappers.h"

static void
module_load_event(void *drcontext, const module_data_t *mod, bool loaded);
static void 
exit_event(void);

DR_EXPORT void
dr_client_main(client_id_t id, int argc, const char *argv[])
{
    tracer_log = fopen("tracer.log", "w");
    assert(tracer_log);
    assert(setvbuf(tracer_log, NULL, _IONBF, 0) == 0);

    hashtable_init_ex(&sockinfo_table, 10, HASH_INTPTR, false, true, free, NULL, NULL);
    drmgr_init();
    drwrap_init();
    dr_register_exit_event(exit_event);
    drmgr_register_module_load_event(module_load_event);
}

static void 
module_load_event(void *drcontext, const module_data_t *mod, bool loaded)
{
    app_pc towrap = (app_pc) dr_get_proc_address(mod->handle, "socket");
    if (towrap != NULL)
        assert(drwrap_wrap(towrap, wrap_pre_socket, wrap_post_socket));

    towrap = (app_pc) dr_get_proc_address(mod->handle, "close");
    if (towrap != NULL)
        assert(drwrap_wrap(towrap, wrap_pre_close, wrap_post_close));

    towrap = (app_pc) dr_get_proc_address(mod->handle, "read");
    if (towrap != NULL)
        assert(drwrap_wrap(towrap, wrap_pre_read, wrap_post_read));

    towrap = (app_pc) dr_get_proc_address(mod->handle, "write");
    if (towrap != NULL)
        assert(drwrap_wrap(towrap, wrap_pre_write, wrap_post_write));

    towrap = (app_pc) dr_get_proc_address(mod->handle, "recv");
    if (towrap != NULL)
        assert(drwrap_wrap(towrap, wrap_pre_recv, wrap_post_recv));

    towrap = (app_pc) dr_get_proc_address(mod->handle, "send");
    if (towrap != NULL)
        assert(drwrap_wrap(towrap, wrap_pre_send, wrap_post_send));

    towrap = (app_pc) dr_get_proc_address(mod->handle, "recvfrom");
    if (towrap != NULL)
        assert(drwrap_wrap(towrap, wrap_pre_recvfrom, wrap_post_recvfrom));

    towrap = (app_pc) dr_get_proc_address(mod->handle, "sendto");
    if (towrap != NULL)
        assert(drwrap_wrap(towrap, wrap_pre_sendto, wrap_post_sendto));
}

static void 
exit_event(void)
{
    fclose(tracer_log);

    hashtable_delete(&sockinfo_table);
    drwrap_exit();
    drmgr_exit();
}
