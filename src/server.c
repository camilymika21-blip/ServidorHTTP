#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 4096
#define BACKLOG  128

static int crear_socket_escucha(const char *puerto)
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
    return sfd;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s puerto\n", argv[0]);
        return EXIT_FAILURE;
    }

    int sfd = crear_socket_escucha(argv[1]);
    if (sfd == -1)
        return EXIT_FAILURE;

    printf("Escuchando en el puerto %s...\n", argv[1]);

    for (;;) {
        struct sockaddr_storage peer_addr;
        socklen_t peer_addrlen = sizeof(peer_addr);
        char host[NI_MAXHOST], service[NI_MAXSERV];
        char buf[BUF_SIZE];

        int cfd = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addrlen);
        if (cfd == -1) {
            perror("accept");
            continue;
        }

        if (getnameinfo((struct sockaddr *)&peer_addr, peer_addrlen,
                        host, NI_MAXHOST, service, NI_MAXSERV,
                        NI_NUMERICHOST | NI_NUMERICSERV) == 0)
            printf("Cliente conectado: %s:%s\n", host, service);

        ssize_t nread = recv(cfd, buf, sizeof(buf) - 1, 0);
        if (nread > 0) {
            buf[nread] = '\0';
            printf("--- Solicitud (%zd bytes) ---\n%s\n", nread, buf);

            const char *body = "Camily Aldaz MiniHTTPd\n";
            char resp[512];
            int n = snprintf(resp, sizeof(resp),
                             "HTTP/1.1 200 OK\r\n"
                             "Content-Type: text/plain\r\n"
                             "Content-Length: %zu\r\n"
                             "Connection: close\r\n"
                             "\r\n"
                             "%s",
                             strlen(body), body);
            send(cfd, resp, (size_t)n, 0);   
        }

        close(cfd);                    
    }

    close(sfd);
    return EXIT_SUCCESS;
}