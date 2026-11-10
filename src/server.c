#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>

struct addrinfo hints, *res, *res0;
int error;
int s;
const char *cause = NULL;


getaddrinfo(const char *hostname, const char *servname, const struct addrinfo *hints, struct addrinfo **res);

int listen(AF_INET, SOCK_STREAM, protocol);
if (listen < 0){
    fprintf(stderr, "ERROR EN LISTEN");
    return -1;
}

if (bind(listen,(struct sockaddr*)&sevr_addr, sizeof(sevr_addr))!=0){
    fprintf(stderr, "Error al asignar puerto asociado")
}



int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int main (int argc, char*argv[])
{
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (arg != 2){
        fprintf(stderr,"Hostname");
        return 1;
    }

    memset (&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    error = getaddrinfo(NULL, AI_PASSIVE, &hints, &res0);
    if (error){
        errx(1, "%s", gai_strerror(error));
    }
    s=-1;
    for (res = res0; res; res= res->ai_next){
        s =socket(res -> ai_family, res->ai_socktype, res -> ai_protocol);
        if(s<0){
            cause="socket";
            continue;
        }
        if(connect(s, res->ai_addr, res->ai_addrlen)<0){
            cause="connect";
            close(s);
            s = -1;
            continue;
        }
        break;
    }
if (s<0){
    err(1,"%s", cause);
}
freeaddrinfo(res0);
}