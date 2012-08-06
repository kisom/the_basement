#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <err.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>


#define MAX_FQDN_LENGTH         (size_t)255

typedef struct sockaddr_in inaddr;
typedef struct sockaddr_in6 in6addr;

extern char     *__progname;

inaddr      *load_in(char *, uint16_t);
in6addr     *load_in6(char *, uint16_t);
void         showpeer(struct sockaddr *, int);
void         reverse_lookup(struct sockaddr *, int);
void         fatal(char *, void *, int, int);


/*
 * print a quick usage message and exit
 */
void
usage()
{
    printf("usage: %s server port\n", __progname);
    exit(EX_USAGE);
}


/*
 * connect to a peer, retrieve its peername, and print out the
 * address and port the computer is connected to.
 */
int
main(int argc, char **argv)
{
    struct sockaddr *addr, *peer;
    socklen_t slen;
    int sockfd = 0;
    int ch;
    int family = AF_INET;
    uint16_t port;

    while ((ch = getopt(argc, argv, "6h")) != -1)
        switch (ch) {
        case '6':
            family = AF_INET6;
            break;
        case 'h':
            /* deliberate fall through */
        default:
            usage();
            /* not reached */
        }

    argv += optind;
    argc -= optind;

    if (argc < 2)
        usage();

    port = (uint16_t)atoi(argv[1]);
    printf("[+] attempting connection to %s:%u\n", argv[0], port);

    sockfd = socket(family, SOCK_STREAM, 0);
    if (-1 == sockfd)
        err(EX_OSERR, "socket() call");

    if (AF_INET == family)
        addr = (struct sockaddr *)load_in(argv[0], port);
    else if (AF_INET6 == family) {
        addr = (struct sockaddr *)load_in6(argv[0], port);
    }

    printf("[+] attempting to connect...\n");
    if (connect(sockfd, (struct sockaddr *)addr, sizeof(*addr)) == -1) {
        fprintf(stderr, "[!] connection failed!\n");
        fatal("connect", addr, sockfd, EX_UNAVAILABLE);
    }
        
    printf("[+] connected.\n");

    printf("[+] getting peername\n");
    if (AF_INET == family)
        slen = sizeof((inaddr *)addr);
    else if (AF_INET6 == family)
        slen = sizeof((in6addr *)addr);

    peer = calloc(1, sizeof(struct sockaddr));
    if (NULL == peer)
        fprintf(stderr, "[!] could not allocate memory for peer address\n");
    else {
        if (-1 == getpeername(sockfd, peer, &slen))
            fprintf(stderr, "[!] failed!\n");
        else {
            showpeer(peer, family);
            reverse_lookup(peer, family);
        }
    }

    printf("[+] finished.\n");

    free(addr);
    addr = NULL;
    close(sockfd);
    return 0;
}


/*
 * load an ipv4 address for the hostname
 */
inaddr *
load_in(char *hostname, uint16_t port)
{
    struct hostent *host;
    inaddr *sa;

    printf("[+] setting up ipv4 address\n");
    sa = calloc(1, sizeof(inaddr));
    if (NULL == sa)
        err(EX_OSERR, "calloc");

    printf("[+] translating host->ip\n");
    if ((host = gethostbyname(hostname)) == NULL)
        err(EX_OSERR, "gethostbyname");

    printf("[+] initialising address structure...\n");

    /* initialise address to ipv4 / sockaddr_in address */
    bcopy(host->h_addr, &sa->sin_addr, host->h_length);
    sa->sin_port = htons(port);
    sa->sin_family = AF_INET;

    printf("[+] ipv4 address set up\n");
    return sa;
}


/*
 * load an ipv6 address for the hostname
 */
in6addr *
load_in6(char *hostname, uint16_t port)
{
    struct addrinfo hints, *res, *cur;
    in6addr *sa;
    
    printf("[+] setting up ipv6 address\n");
    sa = calloc(1, sizeof(in6addr));
    if (NULL == sa)
        err(EX_OSERR, "calloc");

    printf("[+] looking up host->ip\n");
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    if (-1 == getaddrinfo(hostname, NULL, &hints, &res))
        err(EX_OSERR, "getaddrinfo");

    printf("[+] initialising address structure...\n");
    for(cur = res; cur; cur = cur->ai_next) {
        if (cur->ai_socktype != AF_INET6)
            continue;
        bcopy(res->ai_addr, &sa->sin6_addr, res->ai_addrlen);
        sa->sin6_port = htons(port);
        sa->sin6_family = AF_INET6;
        break;
    }

    printf("[+] ipv6 address set up\n");
    return sa;
}

/*
 * print the peer address and port
 */
void
showpeer(struct sockaddr *peer, int family)
{
    char *peer_dq;
    uint16_t peer_port;

    switch (family) {
    case AF_INET:
        peer_dq = inet_ntoa(((inaddr *)peer)->sin_addr);
        peer_port = ntohs(((inaddr *)peer)->sin_port);
        break;
    case AF_INET6:
    /*
        peer_dq = inet_ntoa(((in6addr *)peer)->sin6_addr);
        peer_port = ntohs(((in6addr *)peer)->sin6_port);
     */
        break;
    }

    printf("[+] peer is %s:%u\n", peer_dq, peer_port);
}


/*
 * reverse DNS lookup
 */
void
reverse_lookup(struct sockaddr *peer, int family)
{
    char *fqdn, *service;
    int retval;

    printf("[+] looking up name info for peer\n");

    fqdn = calloc(MAX_FQDN_LENGTH + 1, sizeof(char));
    service = calloc(MAX_FQDN_LENGTH + 1, sizeof(char));
    if (NULL == fqdn || NULL == service) {
        fprintf(stderr, "[!] failed to allocate memory for buffer\n");
        return;
    }

    retval = getnameinfo(peer, sizeof(*peer), fqdn, MAX_FQDN_LENGTH, 
                         service, MAX_FQDN_LENGTH, NI_NOFQDN);
    if (0 == retval) {
        printf("[+] host: %s\n", fqdn);
        printf("    service: %s\n", service);
    } else {
        printf("[!] failed!\n");
    }

    free(service);
    free(fqdn);
}


/*
 * clean up and die
 */
void
fatal(char *msg, void *addr, int sockfd, int exval)
{
    free(addr);
    addr = NULL;

    close(sockfd);
    err(exval, msg);
}
