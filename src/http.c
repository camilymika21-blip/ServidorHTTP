#include <stdio.h>
#include <string.h>
#include "http.h"
#include <sys/socket.h>

#include <server.h>

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

const char *texto_estado(int codigo){
    switch (codigo){
        case 200: return "Ok";
        case 400: return "Bad request";
        case 403: return "Forbidden";
        case 404: return "Not found";
        case 405: return "Method not allowed";
        case 500: return "Internal server error";
        break;
    
    default: return "Unknown";
        break;
    }
    }


void enviar_respuesta(int cfd, int codigo, const char *tipo, const char *cuerpo) {
    char resp[1024];
    int n = snprintf(resp, sizeof(resp),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        codigo, texto_estado(codigo), tipo, strlen(cuerpo), cuerpo);

        if( n > 0 && (size_t)n< sizeof(resp))
        send(cfd, resp, (size_t)n, 0);
}