#ifndef SERVER_H
#define SERVER_H

#define MAX_EVENTS 64    


 * y lo deja no bloqueante. Devuelve el descriptor o -1 si falla. */
int  server_listen(const char *port);

void server_accept(int epfd, int listen_fd);

void server_handle_client(int fd);

#endif