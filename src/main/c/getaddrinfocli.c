#ifdef USERCONFIG
USERCONFIG
#endif

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STR_QUOT_(S) #S
#define STR_QUOT(S) STR_QUOT_(S)


static void printHelp( void ){
    printf("%s%s%s", "  \n"
        "  ", strrchr(__FILE__,'/')+1, " - " STR_QUOT(PROJECT_VERSION) "\n"
        "  \n"
        "  Print result of getaddrinfo [1] to stdout.\n"
        "  \n"
        "  Example:     getaddrinfo  example.com\n"
        "  \n"
        "  [1]: https://pubs.opengroup.org/onlinepubs/9699919799/functions/getaddrinfo.html\n"
        "  \n"
    );
}


int main( int argc, char**argv ){
    int err;
    const char *nodename;
    struct addrinfo *res = NULL, *it;
    if( argc != 2 ){ fprintf(stderr, "EINVAL: Try --help\n"); err = -1; goto endFn; }
    if( !strcmp(argv[1], "--help") ){ printHelp(); err = 0; goto endFn; }
    nodename = argv[1]; assert(nodename != NULL);
    err = getaddrinfo(nodename, NULL, NULL, &res);
    if( err != 0 ){
        const char *ex;
        switch( err ){
        case EAI_AGAIN:      ex = "EAI_AGAIN";      break;
        case EAI_BADFLAGS:   ex = "EAI_BADFLAGS";   break;
        case EAI_FAIL:       ex = "EAI_FAIL";       break;
        case EAI_FAMILY:     ex = "EAI_FAMILY";     break;
        case EAI_MEMORY:     ex = "EAI_MEMORY";     break;
        case EAI_NONAME:     ex = "EAI_NONAME";     break;
        case EAI_SERVICE:    ex = "EAI_SERVICE";    break;
        case EAI_SOCKTYPE:   ex = "EAI_SOCKTYPE";   break;
        case EAI_SYSTEM:     ex = "EAI_SYSTEM";     break;
#ifdef EAI_ADDRFAMILY
        case EAI_ADDRFAMILY: ex = "EAI_ADDRFAMILY"; break;
#endif
#ifdef EAI_NODATA
        case EAI_NODATA:     ex = "EAI_NODATA";     break;
#endif
        default:
            assert(!fprintf(stderr,"ERROR: getaddrinfo(): %d  %s:%d\n", err, __FILE__, __LINE__));
            ex = NULL;
        }
        printf("%s: getaddrinfo(\"%s\")\n", ex, nodename);
        err = -1; goto endFn;
    }
    printf("C: %-6s  %-6s  %-5s  %-4s  %s\n", "AF", "SOCK", "PROTO", "FLGS", "ADDR");
    for( it = res ; it != NULL ; it = it->ai_next ){
        const char *afStr, *typStr, *protoStr;
        switch( it->ai_family ){
        case AF_UNIX  : afStr = "UNIX"  ; break;
        case AF_INET  : afStr = "INET"  ; break;
        case AF_INET6 : afStr = "INET6" ; break;
        case AF_UNSPEC: afStr = "UNSPEC"; break;
        default: assert(!fprintf(stderr,"TODO: ai_family %d\n", it->ai_family)); afStr = NULL;
        }
        switch( it->ai_socktype ){
        case SOCK_STREAM: typStr = "STREAM"; break;
        case SOCK_DGRAM : typStr = "DGRAM" ; break;
        case SOCK_RAW   : typStr = "RAW"   ; break;
        default: assert(!fprintf(stderr, "TODO: ai_socktype %d\n", it->ai_socktype)); typStr = NULL;
        }
        switch( it->ai_protocol ){
        case 0: protoStr = "0"; break;
        case IPPROTO_TCP: protoStr = "TCP"; break;
        case IPPROTO_UDP: protoStr = "UDP"; break;
        default: assert(!fprintf(stderr, "TODO: ai_protocol %d\n", it->ai_protocol)); protoStr = NULL;
        }
        printf("R: %-6s  %-6s  %-5s  0x%02X",
            afStr,
            typStr,
            protoStr,
            it->ai_flags
        );
        switch( it->ai_family ){
        case AF_UNIX: /*TODO anything useful to print here?*/ break;
        case AF_INET: {
            #define ADDR4 ((struct sockaddr_in*)it->ai_addr)
            uint_least32_t ip;
            assert(it->ai_addrlen == 16);
            assert(ADDR4->sin_family == AF_INET);
            ip = ntohl(ADDR4->sin_addr.s_addr);
            printf("  %d.%d.%d.%d", ip >> 24 & 0xFF, ip >> 16 & 0xFF, ip >> 8 & 0xFF, ip & 0xFF);
            break; }
            #undef ADDR4
        case AF_INET6: {
            #define ADDR6 ((struct sockaddr_in6*)it->ai_addr)
            char buf[INET6_ADDRSTRLEN + 1];
            if( inet_ntop(it->ai_family, &ADDR6->sin6_addr, buf, sizeof buf) == NULL ){
                snprintf(buf, sizeof buf, "inet_ntop(): errno=%d", errno);
            }
            printf("  %s", buf);
            break; }
            #undef ADDR6
        default: assert(!fprintf(stderr,"TODO: ai_family %d\n", it->ai_family));
        }
        if( it->ai_canonname != NULL ) printf("  cn=\"%s\"", it->ai_canonname);
        puts("");
    }
    err = 0;
endFn:
    if( res != NULL ) freeaddrinfo(res);
    return !!err;
}

