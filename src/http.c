#include <stdio.h>
#include <string.h>
#include "http.h"

int leer_solicitud(const char *buf, request_t *req){
    if(sscanf(buf, "%7s %1023s %15s", req->method, req->uri, req->version) != 3)
    return  400;    

    if(req->uri[0]!='/')
    return 400;

    if(strcmp(req->version, "HTTP/1.1") != 0 && strcmp(req->version, "HTTP/1.0") !=0)
    return 400;

    if(strcmp(req->method, "GET")!=0)
    return 405;

    return 200;
}
