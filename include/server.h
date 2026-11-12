#ifndef SERVER_H
#define SERVER_H

#define MAX_EVENTS 64    

int  crear_socket_escucha(const char *puerto);

void aceptar_clientes(int epfd, int sfd);

void atender_cliente(int cfd);

#endif  