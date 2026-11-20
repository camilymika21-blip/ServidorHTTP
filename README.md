MiniHTTPd

Servidor básico escrito en C para linux, sirve archivos estáticos (HTML,CSS e imágenes) y atiende a varios clientes a la vez con un solo hilo usando epoll y sockets no bloqueantes

Proyecto para la materia de Computación Distribuida 2026-B

Características 
   Sirve archivos estáticos desde la capeta www/
   Procesa el método get y analiza la línea de sollicitud (Método, URI y versión)
   Atiende múltiples clientes con epoll dirigida por eventos, un solo hilo
   Envía el Content-Type correcto segun la extensión de archivo
   Responde con códigos de estado 200, 400, 403, 404, 405 y 500
   Cierra la conexión despues de cada respuesta (close())

Estructura

  minihttpd/
├── Makefile
├── README.md
├── include/
│   ├── http.h
│   ├── server.h
│   ├── mime.h
│   └── files.h
├── src/
│   ├── main.c      # bucle de eventos (epoll)
│   ├── server.c    # socket de escucha, aceptar y atender clientes
│   ├── http.c      # parsing de la solicitud GET y generación de respuestas
│   ├── mime.c      # tipos de contenido según la extensión
│   └── files.c     # acceso seguro a los archivos estáticos
└── www/
    ├── index.html
    ├── style.css
    └── Logoepn.png 

Compilación

Requiere gcc y make en linux
Makefile incluido contiene todo lo necesario para compilar el proyecto con un solo comando make

Pruebas 

 # Página principal (200, text/html)
curl -i http://127.0.0.1:8080/

# Hoja de estilos (200, text/css)
curl -i http://127.0.0.1:8080/style.css

# Imagen (200, image/png); se descarta el cuerpo y se muestran las cabeceras
curl -i http://127.0.0.1:8080/image.png --output /dev/null -D -

# Archivo inexistente (404)
curl -i http://127.0.0.1:8080/no-existe.html

# Método no permitido (405)
curl -i -X POST http://127.0.0.1:8080/

# Intento de directory traversal (403 o 404, nunca el archivo del sistema)
curl -i --path-as-is http://127.0.0.1:8080/../../etc/passwd

Tipos de MIME 
Extensión        Content-Type
.html              text/html
.css               text/css
.png               image/png
otras              application/octet-stream

Seguridad
Directory traversal la ruta pedida se resuelve con realpath() y se verifica que quede dentro de la raiz www/ de lo contrario se responde 404

Métodos Inválidos todo método distinto de GET se rechaza con 405

Solicitadas malformadas se rechaza con 400

Diseño 
El servidor funciona con un solo hilo

    Se crea un socket TCP se escucha no bloqueante (getaddrinfo, socket, bind, listen)
    Se registra en una lista epoll
    El bucle principalespera eventos con epoll_wait si el socket esta listo es el de escucha, se acepta todos los clientes pendientes y se registra en epoll, si es el de un cliente se lee su solcitud se responde y se cierra la conexión 

Limitaciones

    No soporta conexiones persistenres cada respuesta usa close()
    Solo implementa el método GET
    