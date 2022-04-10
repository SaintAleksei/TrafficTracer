#ifndef SOURCES_WRAPPERS
#define SOURCES_WRAPPERS

#include "drwrap.h"
#include <stdio.h>

void 
wrap_pre_socket(void *wrapctx, OUT void **userdata);
void
wrap_post_socket(void *wrapctx, void *userdata);

void 
wrap_pre_close(void *wrapctx, OUT void **userdata);
void
wrap_post_close(void *wrapctx, void *userdata);

void
wrap_pre_read(void *wrapctx, OUT void **userdata);
void
wrap_post_read(void *wrapctx, void *userdata);

void
wrap_pre_write(void *wrapctx, OUT void **userdata);
void
wrap_post_write(void *wrapctx, void *userdata);

void
wrap_pre_recv(void *wrapctx, OUT void **userdata);
void
wrap_post_recv(void *wrapctx, void *userdata);

void
wrap_pre_send(void *wrapctx, OUT void **userdata);
void
wrap_post_send(void *wrapctx, void *userdata);

void
wrap_pre_recvfrom(void *wrapctx, OUT void **userdata);
void
wrap_post_recvfrom(void *wrapctx, void *userdata);

void
wrap_pre_sendto(void *wrapctx, OUT void **userdata);
void
wrap_post_sendto(void *wrapctx, void *userdata);

extern hashtable_t sockinfo_table;
extern FILE *tracer_log;

#endif
