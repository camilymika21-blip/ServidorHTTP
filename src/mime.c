#include <string.h>
#include <strings.h>

#include "mime.h"

static const struct {
    const char *ext;
    const char *tipo;
} tabla_mime[] ={
      {".html", "text/html"},
    {".css",  "text/css"},
    {".js",   "application/javascript"},
    {".png",  "image/png"},
    {".jpg",  "image/jpeg"},
};

const char *tipo_mime(const char *ruta)
{
    if (strcmp(ruta, "/") == 0)          
        return "text/html";

    const char *punto = strrchr(ruta, '.');
    if (punto == NULL)
        return "application/octet-stream";

     for (size_t i = 0; i < sizeof(tabla_mime) / sizeof(tabla_mime[0]); i++)
        if (strcasecmp(punto, tabla_mime[i].ext) == 0)
            return tabla_mime[i].tipo;
    
    return "application/octet-stream";
}
