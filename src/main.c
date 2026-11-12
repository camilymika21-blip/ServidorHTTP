#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>

#include <server.h>


int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s puerto\n", argv[0]);
        return EXIT_FAILURE;
    }
 
    int sfd = crear_socket_escucha(argv[1]);
    if (sfd == -1)
        return EXIT_FAILURE;
 
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }
 
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = sfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &ev) == -1) {
        perror("epoll_ctl: sfd");
        return EXIT_FAILURE;
    }
 
    printf("Escuchando en el puerto %s...\n", argv[1]);
    fflush(stdout);
 
    struct epoll_event events[MAX_EVENTS];
    for (;;) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            if (errno == EINTR)
                continue;
            perror("epoll_wait");
            break;
        }
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == sfd)
                aceptar_clientes(epfd, sfd);
            else
                atender_cliente(events[i].data.fd);
        }
    }
 
    close(epfd);
    close(sfd);
    return EXIT_SUCCESS;
}