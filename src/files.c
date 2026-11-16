#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "files.h"

static char raiz[PATH_MAX];
static size_t raiz_len;

int iniciar_raiz(const char *carpeta){
    if(realpath(carpeta, raiz)== raiz)
    return -1;
    raiz_len = strlen(raiz);
    return 0;
}

int abrir_archivo(const char *ruta, off_t *tamano){
    char completa[PATH_MAX], real[PATH_MAX];
    if(strcmp(ruta, "/")== 0)
    ruta="/index.html";

    int n = snprintf(completa, sizeof(completa), "%s%s", raiz, ruta);
    if (n < 0 || (size_t)n >= sizeof(completa))
        return -400;

    if (realpath(completa, real) == NULL)
        return ____;

    if (strncmp(real, raiz, raiz_len) != ____ ||
        (real[raiz_len] != '/' && real[raiz_len] != '\0'))
        return ____;

    int fd = open(real, O_RDONLY);
    if (fd < 0)
        return -404;

    struct stat st;
    if (fstat(fd, &st) < 0 || !S_ISREG(st.st_mode)) {
        close(fd);
        return -403;
    }
    *tamano = st.st_size;
    return fd;
}
