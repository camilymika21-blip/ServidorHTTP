#ifndef HTTP_H
#define HTTP_H
#define MAX_METHOD 8
#define MAX_URI 1024
#define MAX_VERSION 16

typedef struct {
    char method[MAX_METHOD];
    char uri[MAX_URI];
    char version[MAX_VERSION];
} request_t;

int leer_solicitud(const char *buf, request_t *req);

#endif


