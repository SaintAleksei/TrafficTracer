#ifndef SOURCES_SOCKINFO
#define SOURCES_SOCKINFO

#include <stdint.h>

struct sockinfo
{
    int64_t domain;
    int64_t type;
    int64_t sockfd;
};

const char*
sockinfo_resolve_domain(int domain);

const char*
sockinfo_resolve_type(int type);

#endif
