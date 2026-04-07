//
// Created by adazu on 05/04/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#include "common.h"

#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include "claves.h"




int conectar() { //al servidor de PARTE DEL CLIENTE
    char *ip;
    char *port_string;
    int port;
    int sd;
    struct hostent *he;    //info de hosts
    struct sockaddr_in server_addr;  //dir del socket

    ip = getenv("IP_TUPLAS"); //dado por el enunciado
    port_string = getenv("PORT_TUPLAS"); //dado por el enunciado
    port = atoi(port_string);

    if (ip == NULL) { printf("Error IP"); return -1;}   //manejo de errores
    if (port_string == NULL) { printf("Error Port_String"); return -1;}  //manejo de errores

    //partee del socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {printf("Error Socket"); return -1;}

    //ajustamos la direccción del servidor
    memset((char *) &server_addr, 0, sizeof(server_addr)); //limpio la memoria y pongo a 0
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    //y finalmente convertimos
    he = gethostbyname(ip);
    if (he == NULL) {printf("Error Hostent"); close (sd);return -1;}
    memcpy((char *) &server_addr.sin_addr, he->h_addr, sizeof(server_addr.sin_addr)); //copio la direccion IP

    //conectamos
    if (connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Error Conectar"); close (sd);return -1;
    }

    return sd;
}




//
//funciones de claves.hASDASDASDA
//
int destroy(void) {
    int a;
    int b;
    int op;
    int resultado;
    int sockfd = conectar();
    if (sockfd == -1) {printf("error conectar socket"); return -1;}

    op = 0; //la operacion de DESTROY sera asginada al numero 0

    //primero enviamos
    a = send_message(sockfd, (char *)&op, sizeof(int));
    if (a == -1){printf("error enviar socket"); close(sockfd); return -1;}
    //la recibo
    b = receive_message(sockfd, (char *)&resultado, sizeof(int));
    if (b == -1){printf("error recibir socket"); close(sockfd); return -1;}

    close(sockfd);
    return resultado;
}

int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    int b;
    int op;
    int resultado;
    int sockfd = conectar();
    if (sockfd == -1) {printf("error conectar socket"); return -1;}

    op = 1; //la operacion de SET sera asginada al numero 1

    //a esta operacion le tenemos q pasar valores, asiq enviamos 1 a 1
    send_message(sockfd, (char *)&op, sizeof(int));

    send_message(sockfd, key, 256);
    send_message(sockfd, value1, 256);


    send_message(sockfd, (char *)&N_value2, sizeof(int));
    send_message(sockfd, (char *)V_value2, N_value2 * sizeof(float));
    send_message(sockfd, (char *)&value3, sizeof(struct Paquete));

    //recibimos
    b = receive_message(sockfd, (char *)&resultado, sizeof(int));
    if (b == -1){printf("error recibir socket"); close(sockfd); return -1;}

    close(sockfd);
    return resultado;
}

int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
    int b;
    int op;
    int resultado;
    int sockfd = conectar();
    if (sockfd == -1) {printf("error conectar socket"); return -1;}

    op = 2; //la operacion de GET sera asginada al numero 2

    //le pasamos 1 qa 1
    send_message(sockfd, (char *)&op, sizeof(int));
    send_message(sockfd, key, 256);

    //la recibo
    b = receive_message(sockfd, (char *)&resultado, sizeof(int));
    if (b == -1){printf("error recibir socket"); close(sockfd); return -1;}

    //despues de recibirla, si todo es correcto debemos copiar
    if (resultado == 0) { //es decir, no hay errores
        receive_message(sockfd, value1, 256);
        receive_message(sockfd, (char *)N_value2, sizeof(int));
        receive_message(sockfd, (char *)V_value2, (*N_value2) * sizeof(float));
        receive_message(sockfd, (char *)value3, sizeof(struct Paquete));
    }

    //cerramos
    close(sockfd);
    return resultado;
}

int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    int b;
    int op;
    int resultado;
    int sockfd = conectar();
    if (sockfd == -1) {printf("error conectar socket"); return -1;}

    op = 3; //la operacion de MODIFY sera asginada al numero 3

    //a esta operacion le tenemos q pasar valores, pasamos 1 a 1
    send_message(sockfd, (char *)&op, sizeof(int));
    send_message(sockfd, key, 256);
    send_message(sockfd, value1, 256);
    send_message(sockfd, (char *)&N_value2, sizeof(int));
    send_message(sockfd, (char *)V_value2, N_value2 * sizeof(float));
    send_message(sockfd, (char *)&value3, sizeof(struct Paquete));

    //la recibo
    b = receive_message(sockfd, (char *)&resultado, sizeof(int));
    if (b == -1){printf("error recibir socket"); close(sockfd); return -1;}

    close(sockfd);
    return resultado;
}

int delete_key(char *key) {
    int b;
    int op;
    int resultado;
    int sockfd = conectar();
    if (sockfd == -1) {printf("error conectar socket"); return -1;}

    op = 4; //la operacion de DELETE sera asginada al numero 4

    //paso args y envio
    send_message(sockfd, (char *)&op, sizeof(int));
    send_message(sockfd, key, 256);

    //la recibo
    b = receive_message(sockfd, (char *)&resultado, sizeof(int));
    if (b == -1){printf("error recibir socket"); close(sockfd); return -1;}

    close(sockfd);
    return resultado;
}

int exist(char *key) {
    int b;
    int op;
    int resultado;
    int sockfd = conectar();
    if (sockfd == -1) {printf("error conectar socket"); return -1;}

    op = 5; //la operacion de EXISTS sera asginada al numero 5

    //paso los args necesarios y envio
    send_message(sockfd, (char *)&op, sizeof(int));
    send_message(sockfd, key, 256);

    //la recibo
    b = receive_message(sockfd, (char *)&resultado, sizeof(int));
    if (b == -1){printf("error recibir socket"); close(sockfd); return -1;}

    close(sockfd);
    return resultado;
}
