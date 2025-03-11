//SERVIDOR
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000

// Funciones para conversión
float celsiusAFahrenheit(float c) {
    return (c * 9.0f / 5.0f) + 32.0f;
}

float fahrenheitACelsius(float f) {
    return (f - 32.0f) * 5.0f / 9.0f;
}

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int sock_listen, sock_conn;
    struct sockaddr_in serv_addr;

    // Crear socket
    sock_listen = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_listen < 0)
        error("Error al abrir el socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons(PORT);

    // Hacemos bind
    if (bind(sock_listen, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Error en bind");

    // Escuchamos
    listen(sock_listen, 5);
    printf("Servidor conversor escuchando en puerto %d...\n", PORT);

    while (1) {
        sock_conn = accept(sock_listen, NULL, NULL);
        if (sock_conn < 0) {
            perror("Error en accept");
            continue;
        }
        printf("Cliente conectado.\n");

        
        int terminar = 0;
        while (!terminar) {
            char buffer[256];
            int ret = read(sock_conn, buffer, sizeof(buffer) - 1);
            if (ret <= 0) {
                break;
            }
            buffer[ret] = '\0';

            
            char *p = strtok(buffer, "/");
            int codigo = atoi(p);
            if (codigo == 0) {
                // Desconexión
                terminar = 1;
            } else {
                p = strtok(NULL, "/");
                float valor = atof(p);
                float resultado = 0;
                char respuesta[64];

                if (codigo == 1) {
                    // Celsius -> Fahrenheit
                    resultado = celsiusAFahrenheit(valor);
                    sprintf(respuesta, "%.2f C son %.2f F", valor, resultado);
                }
                else if (codigo == 2) {
                    // Fahrenheit -> Celsius
                    resultado = fahrenheitACelsius(valor);
                    sprintf(respuesta, "%.2f F son %.2f C", valor, resultado);
                }
                else {
                    // Código desconocido
                    sprintf(respuesta, "Error: código %d no válido", codigo);
                }

                // Enviamos respuesta
                write(sock_conn, respuesta, strlen(respuesta));
            }
        }
        close(sock_conn);
        printf("Cliente desconectado.\n");
    }

    close(sock_listen);
    return 0;
}

//CLIENTE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 8000

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    // Crear socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        error("Error al abrir el socket");

   
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr, "Error, host no encontrado\n");
        exit(0);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(PORT);

    // Conectar
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Error en connect");

  
    char mensaje[64];
    strcpy(mensaje, "1/20.5");
    write(sock, mensaje, strlen(mensaje));

  
    char buffer[256];
    int ret = read(sock, buffer, 255);
    buffer[ret] = '\0';
    printf("Respuesta: %s\n", buffer);

    
    strcpy(mensaje, "2/80.2");
    write(sock, mensaje, strlen(mensaje));

    ret = read(sock, buffer, 255);
    buffer[ret] = '\0';
    printf("Respuesta: %s\n", buffer);

    // Desconectar (enviando "0/")
    strcpy(mensaje, "0/");
    write(sock, mensaje, strlen(mensaje));

    close(sock);
    return 0;
}
