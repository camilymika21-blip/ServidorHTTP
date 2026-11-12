#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>

#include <server.h>

#define BUF_SIZE 4096
#define BACKLOG  128
#define MAX_EVENTS 64

static int set_nonblocking(int fd){
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    return -1;
return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int crear_socket_escucha(const char *puerto)
{
    struct addrinfo hints, *result, *rp;
    int sfd = -1, s, opt = 1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;     
    hints.ai_socktype = SOCK_STREAM;   /* TCP (antes era SOCK_DGRAM) */
    hints.ai_flags    = AI_PASSIVE;    

    s = getaddrinfo(NULL, puerto, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                     

        close(sfd);
    }
    freeaddrinfo(result);

    if (rp == NULL) {
        fprintf(stderr, "No se pudo hacer bind en el puerto %s\n", puerto);
        return -1;
    }

    if (listen(sfd, BACKLOG) == -1) {  
        perror("listen");
        close(sfd);
        return -1;
    }

    if(set_nonblocking(sfd)==-1){
        perror("fcntl");
        close(sfd);
        return -1;
    }

    return sfd;
}

void aceptar_clientes(int epfd, int sfd){

    for (;;) {
        struct sockaddr_storage peer_addr;
        socklen_t peer_addrlen = sizeof(peer_addr);
        char host[NI_MAXHOST], service[NI_MAXSERV];
        char buf[BUF_SIZE];

        int cfd = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addrlen);
        if (cfd == -1) {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            break;
            if(errno == EINTR || errno == ECONNABORTED)
            continue;
            perror("aceptado");
            break;
        }

        if (getnameinfo((struct sockaddr *)&peer_addr, peer_addrlen,
                        host, NI_MAXHOST, service, NI_MAXSERV,
                        NI_NUMERICHOST | NI_NUMERICSERV) == 0)
            printf("Cliente conectado: %s:%s\n", host, service);

        if(set_nonblocking(cfd)==-1){
            close(cfd);
            continue;
        }
        
        struct epoll_event ev;
        memset(&ev, 0, sizeof(ev));
        ev.events=EPOLLIN;
        ev.data.fd=cfd;
        if(epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev)==-1){
            perror("epoll_ctl= cliente");
            close(cfd);
        }                 
    }
}

void atender_cliente(int cfd)
{
    char buf[BUF_SIZE];
 
    ssize_t nread = recv(cfd, buf, sizeof(buf) - 1, 0);
    if (nread < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
        return;                       
 
    if (nread > 0) {
        buf[nread] = '\0';
        printf("Solicitud (%zd bytes)\n%s\n", nread, buf);
 
        const char *body = "Camily Aldaz MiniHTTPd\n";
        char resp[512];
        int n = snprintf(resp, sizeof(resp),
                         "HTTP/1.1 200 OK\r\n"
                         "\r\n"
                         "%s",body);
                send(cfd, resp, (ssize_t)n, 0);
    }
 
    close(cfd);                        
}
 
