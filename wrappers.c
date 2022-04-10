#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "dr_api.h"
#include "hashtable.h"
#include "wrappers.h"
#include "sockinfo.h"

#define DATA_SEPARATOR "########################################\n"

hashtable_t sockinfo_table;
FILE *tracer_log = NULL;

void 
wrap_pre_socket(void *wrapcxt, OUT void **userdata)
{
    assert(wrapcxt);
    assert(tracer_log);

    int64_t domain = (int64_t) drwrap_get_arg(wrapcxt, 0);
    int64_t type = (int64_t) drwrap_get_arg(wrapcxt, 1);
    int64_t protocol = (int64_t) drwrap_get_arg(wrapcxt, 2);

    fprintf(tracer_log, "socket(%s, %s, %ld) = ", 
                        sockinfo_resolve_domain(domain),
                        sockinfo_resolve_type(type),
                        type);

    struct sockinfo *new_sock = malloc(sizeof(*new_sock));
    assert(new_sock);

    new_sock->domain = domain;
    new_sock->type = type;

    *userdata = new_sock;
}

void 
wrap_post_socket(void *wrapcxt, void *userdata)
{
    assert(wrapcxt);

    struct sockinfo *new_sock = userdata;

    new_sock->sockfd = (int64_t) drwrap_get_retval(wrapcxt);

    fprintf(tracer_log, "%ld;\n", new_sock->sockfd);

    if (new_sock->sockfd != -1)
        hashtable_add(&sockinfo_table, (void *) new_sock->sockfd, new_sock);
    else
        free(new_sock);
}

void
wrap_pre_close(void *wrapcxt, OUT void **userdata)
{
    assert(wrapcxt);

    *userdata = drwrap_get_arg(wrapcxt, 0);

    fprintf(tracer_log, "close(%ld) = ", (int64_t) *userdata);
}

void 
wrap_post_close(void *wrapcxt, void *userdata)
{
    assert(wrapcxt);
    
    int64_t retval = (int64_t) drwrap_get_retval(wrapcxt);
    
    fprintf(tracer_log, "%ld;\n", retval);

    if (retval != -1)
        hashtable_remove(&sockinfo_table, userdata);
}

void
wrap_pre_write(void *wrapcxt, OUT void **userdata)
{
    assert(wrapcxt);

    int64_t fd = (int64_t) drwrap_get_arg(wrapcxt, 0);
    void *buf  = drwrap_get_arg(wrapcxt, 1);
    uint64_t count = (uint64_t) drwrap_get_arg(wrapcxt, 2);

    fprintf(tracer_log, "write(%ld, %p, %lu) = ",
                        fd, buf, count);

    if (hashtable_lookup(&sockinfo_table, (void *) fd))
        *userdata = buf;
    else
        *userdata = NULL;
}

void
wrap_post_write(void *wrapcxt, void *userdata)
{
    assert(wrapcxt);

    int64_t retval = (int64_t) drwrap_get_retval(wrapcxt);

    fprintf(tracer_log, "%ld;\n", retval);

    if (retval != -1 && userdata)
    {
        fprintf(tracer_log, DATA_SEPARATOR);
        fwrite(userdata, 1, retval, tracer_log);
        fprintf(tracer_log, "\n" DATA_SEPARATOR);
    }
}

void
wrap_pre_read(void *wrapcxt, OUT void **userdata)
{
    assert(wrapcxt);
    
    int64_t fd = (int64_t) drwrap_get_arg(wrapcxt, 0);
    void *buf  = drwrap_get_arg(wrapcxt, 1);
    int64_t count = (int64_t) drwrap_get_arg(wrapcxt, 2);

    fprintf(tracer_log, "read(%ld, %p, %lu) = ",
                        fd, buf, count);

    if (hashtable_lookup(&sockinfo_table, (void *) fd))
        *userdata = buf;
    else
        *userdata = NULL;
}

void
wrap_post_read(void *wrapcxt, void *userdata)
{
    assert(wrapcxt);

    int64_t retval = (int64_t) drwrap_get_retval(wrapcxt);

    fprintf(tracer_log, "%ld;\n", retval);

    if (retval != -1 && userdata)
    {
        fprintf(tracer_log, DATA_SEPARATOR);
        fwrite(userdata, 1, retval, tracer_log);
        fprintf(tracer_log, "\n" DATA_SEPARATOR);
    }
}

void
wrap_pre_send(void *wrapcxt, OUT void **userdata)
{
    assert(wrapcxt);
    assert(tracer_log);
        
    int64_t sockfd = (int64_t) drwrap_get_arg(wrapcxt, 0);
    void *buf = drwrap_get_arg(wrapcxt, 1);
    uint64_t len = (uint64_t) drwrap_get_arg(wrapcxt, 2);
    int64_t flags = (int64_t) drwrap_get_arg(wrapcxt, 3);

    fprintf(tracer_log, "send(%ld, %p, %lu, %ld) = ",
                        sockfd, buf, len, flags);
    
    if (hashtable_lookup(&sockinfo_table, (void *) sockfd))
        *userdata = buf;
    else
        *userdata = NULL;
}

void
wrap_post_send(void *wrapcxt, void *userdata)
{
    assert(wrapcxt);
    assert(tracer_log);

    int64_t retval = (int64_t) drwrap_get_retval(wrapcxt);

    fprintf(tracer_log, "%ld;\n", retval);

    if (retval != -1 && userdata)
    {
        fprintf(tracer_log, DATA_SEPARATOR);
        fwrite(userdata, 1, retval, tracer_log);
        fprintf(tracer_log, "\n" DATA_SEPARATOR);
    }
}

void
wrap_pre_recv(void *wrapcxt, OUT void **userdata)
{
    assert(wrapcxt);
    assert(tracer_log);
        
    int64_t sockfd = (int64_t) drwrap_get_arg(wrapcxt, 0);
    void *buf = drwrap_get_arg(wrapcxt, 1);
    uint64_t len = (uint64_t) drwrap_get_arg(wrapcxt, 2);
    int64_t flags = (int64_t) drwrap_get_arg(wrapcxt, 3);

    fprintf(tracer_log, "recv(%ld, %p, %lu, %ld) = ",
                        sockfd, buf, len, flags);
    
    if (hashtable_lookup(&sockinfo_table, (void *) sockfd))
        *userdata = buf;
    else
        *userdata = NULL;
}

void
wrap_post_recv(void *wrapcxt, void *userdata)
{
    assert(wrapcxt);
    assert(tracer_log);

    int64_t retval = (int64_t) drwrap_get_retval(wrapcxt);

    fprintf(tracer_log, "%ld;\n", retval);

    if (retval != -1 && userdata)
    {
        fprintf(tracer_log, DATA_SEPARATOR);
        fwrite(userdata, 1, retval, tracer_log);
        fprintf(tracer_log, "\n" DATA_SEPARATOR);
    }
}

void
wrap_pre_sendto(void *wrapcxt, OUT void **userdata)
{
    assert(wrapcxt);
    assert(tracer_log);
        
    int64_t sockfd = (int64_t) drwrap_get_arg(wrapcxt, 0);
    void *buf = drwrap_get_arg(wrapcxt, 1);
    uint64_t len = (uint64_t) drwrap_get_arg(wrapcxt, 2);
    int64_t flags = (int64_t) drwrap_get_arg(wrapcxt, 3);

    fprintf(tracer_log, "sendto(%ld, %p, %lu, %ld, ..., ...) = ",
                        sockfd, buf, len, flags);
    
    if (hashtable_lookup(&sockinfo_table, (void *) sockfd))
        *userdata = buf;
    else
        *userdata = NULL;
}

void
wrap_post_sendto(void *wrapcxt, void *userdata)
{
    assert(wrapcxt);
    assert(tracer_log);

    int64_t retval = (int64_t) drwrap_get_retval(wrapcxt);

    fprintf(tracer_log, "%ld;\n", retval);

    if (retval != -1 && userdata)
    {
        fprintf(tracer_log, DATA_SEPARATOR);
        fwrite(userdata, 1, retval, tracer_log);
        fprintf(tracer_log, "\n" DATA_SEPARATOR);
    }
}

void
wrap_pre_recvfrom(void *wrapcxt, OUT void **userdata)
{
    assert(wrapcxt);
    assert(tracer_log);
        
    int64_t sockfd = (int64_t) drwrap_get_arg(wrapcxt, 0);
    void *buf = drwrap_get_arg(wrapcxt, 1);
    uint64_t len = (uint64_t) drwrap_get_arg(wrapcxt, 2);
    int64_t flags = (int64_t) drwrap_get_arg(wrapcxt, 3);

    fprintf(tracer_log, "recv(%ld, %p, %lu, %ld, ..., ...) = ",
                        sockfd, buf, len, flags);
    
    if (hashtable_lookup(&sockinfo_table, (void *) sockfd))
        *userdata = buf;
    else
        *userdata = NULL;
}

void
wrap_post_recvfrom(void *wrapcxt, void *userdata)
{
    assert(wrapcxt);
    assert(tracer_log);

    int64_t retval = (int64_t) drwrap_get_retval(wrapcxt);

    fprintf(tracer_log, "%ld;\n", retval);

    if (retval != -1 && userdata)
    {
        fprintf(tracer_log, DATA_SEPARATOR);
        fwrite(userdata, 1, retval, tracer_log);
        fprintf(tracer_log, "\n" DATA_SEPARATOR);
    }
}
