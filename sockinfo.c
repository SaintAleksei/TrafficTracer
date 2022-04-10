#include "sockinfo.h"
#include <sys/socket.h>
#include <sys/types.h>

const char *
sockinfo_resolve_domain(int domain)
{
    switch(domain)
    {
        case AF_UNIX:
            return "AF_UNIX";
        case AF_AX25:
            return "AF_AX25";
        case AF_IPX:
            return "AF_IPX";
        case AF_DECnet:
            return "AF_DECnet";
        case AF_KEY:
            return "AF_KEY";
        case AF_NETLINK:
            return "AF_NETLINK";
        case AF_PACKET:
            return "AF_PACKET";
        case AF_RDS:
            return "AF_RDS";
        case AF_PPPOX:
            return "AF_PPPOX";
        case AF_LLC:
            return "AF_LLC";
        case AF_IB:
            return "AF_IB";
        case AF_MPLS:
            return "AF_MPLS";
        case AF_CAN:
            return "AF_CAN";
        case AF_TIPC:
            return "AF_TIPC";
        case AF_BLUETOOTH:
            return "AF_BLUETOOTH";
        case AF_ALG:
            return "AF_ALG";
        case AF_VSOCK:
            return "AF_VSOCK";
        case AF_KCM:
            return "AF_KCM";
        case AF_XDP:
            return "AF_XDP";
        case AF_INET:
            return "AF_INET";
        case AF_INET6:
            return "AF_INET6";
        default: 
            return "Unsupported socket domain";
    }
}

const char*
sockinfo_resolve_type(int type)
{
    type &= ~(SOCK_NONBLOCK | SOCK_CLOEXEC);

    switch(type)
    {
        case SOCK_STREAM:
            return "SOCK_STREAM";
        case SOCK_DGRAM:
            return "SOCK_DGRAM";
        case SOCK_SEQPACKET:
            return "SOCK_SEQPACKET";
        case SOCK_RAW:
            return "SOCK_RAW";
        case SOCK_RDM:
            return "SOCK_RDM";
        case SOCK_PACKET:
            return "SOCK_PACKET";
        default:
            return "Unsupported socket type";
    } 
}
