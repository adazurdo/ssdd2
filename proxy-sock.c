//
// Created by adazu on 05/04/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include "claves.h"
#include <sys/socket.h>



int conectar() { //al servidor
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


//creo funcion/es auxiloares para no repetior codigo de enviar y recibir

//
//funciones de claves.h
//
int destroy(void) {

}

int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {

}

int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {

}

int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {

}

int delete_key(char *key) {

}

int exist(char *key) {

}
