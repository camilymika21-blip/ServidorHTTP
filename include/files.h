#ifndef FILES_H
#define FILES_H

#include <sys/types.h>

int iniciar_raiz(const char *carpeta);

int abrir_archivo(const char *ruta, off_t *tamano);

#endif