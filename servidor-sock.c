//
// Created by adazu on 05/04/2026.
//

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "claves.h"


//hago las equivalencias en la PARTE DEL SERVIDOR
int server_socket (int port) {
    int socket_sd;
    int a;
    int b;
    int valor;
    struct sockaddr_in server_addr; //direccion del sockt

    //priemro hay q crearlo
    socket_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_sd == -1) {printf("Error creando socket"); return -1;}

    //opcion de re3inicio
    valor = 1;
    setsockopt(socket_sd, SOL_SOCKET, SO_REUSEADDR, (char *) &valor, sizeof(valor));

    //trabajamos con la dir del servidor
    memset((char *) &server_addr, 0, sizeof(server_addr)); //primero de todo pongo todo a 0 por si acaso
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    //linkeamos el socket al pueto correspondiente
    a = bind(socket_sd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (a == -1) {printf("Error linkeando socket"); close(socket_sd); return -1;}

    //escuhanmos
    b = listen(socket_sd, SOMAXCONN);
    if (b == -1) {printf("Error escuhando socket"); close(socket_sd); return -1;}

    return socket_sd;

}

int server_accept(int socket) {
    int socker_cliente;
    struct sockaddr_in cliente_addr;
    socklen_t size;

    //print de comprobacion
    printf("Esperando...");

    size = sizeof(cliente_addr);
    socker_cliente = accept(socket, (struct sockaddr *) &cliente_addr, &size);
    if (socker_cliente == -1) {printf("Error aceptando socket"); return -1;}

    //si todo va bien: + comprobaciones
    printf("Conexion correcta de %s:%d\n", inet_ntoa(cliente_addr.sin_addr), ntohs(cliente_addr.sin_port));
    return socker_cliente;

}

//funcion para un thread
void *gestion_cliente(void *arg) {
    int socket_cliente;
    int op;
    int resultado;
    int recibido;

    char key[256];
    char value1[256];
    int N_value2;
    float V_value2[32];
    struct Paquete value3;

    socket_cliente = *((int *) arg);
    free(arg);

    //miro q codigo recibo
    recibido = receive_message(socket_cliente, (char *) &op, sizeof(int));
    if (recibido == -1) {printf("Error recibido"); close(socket_cliente); pthread_exit(NULL);}

    //si todo va bien:
    printf("Opcion %d\n", op);


    //ahora creo un switch capaz de manejar todas las operaciones posibles segun su numerp
    switch (op) {
        case 0: //destrot
            resultado = destroy();
            send_message(socket_cliente, (char *) &resultado, sizeof(int));
            break;
        case 1: //set
            //primero tengo q ver todos los cambios que me han mandado
            receive_message(socket_cliente, key, 256);
            receive_message(socket_cliente, value1, 256);
            receive_message(socket_cliente, (char*)&N_value2, sizeof(int));
            receive_message(socket_cliente, (char*)&V_value2, N_value2 * sizeof(float));
            // Recibir struct Paquete campo por campo (independiente del lenguaje)
            receive_message(socket_cliente, (char*)&value3.x, sizeof(int));
            receive_message(socket_cliente, (char*)&value3.y, sizeof(int));
            receive_message(socket_cliente, (char*)&value3.z, sizeof(int));

            //despues de recibir todo tengo qe llamar a la funcion
            resultado = set_value(key, value1, N_value2, V_value2, value3);
            //lo m ando
            send_message(socket_cliente, (char *) &resultado, sizeof(int));
            break;
        case 2: //get
            //miro lo que me han mandado
            receive_message(socket_cliente, key, 256);

            //llamo a la func
            resultado = get_value(key, value1, &N_value2, V_value2, &value3);

            //devuelvo el resultado
            send_message(socket_cliente, (char *) &resultado, sizeof(int));

            //si todo va bien, envio resultados
            if (resultado == 0) {
                send_message(socket_cliente, value1, 256);
                send_message(socket_cliente, (char*)&N_value2, sizeof(int));
                send_message(socket_cliente, (char*)&V_value2, N_value2 * sizeof(float));
                // Enviar struct Paquete campo por campo (independiente del lenguaje)
                send_message(socket_cliente, (char*)&value3.x, sizeof(int));
                send_message(socket_cliente, (char*)&value3.y, sizeof(int));
                send_message(socket_cliente, (char*)&value3.z, sizeof(int));
            }
            break;

        case 3: //modify
            //primero tengo q ver todos los cambios que me han mandado
            receive_message(socket_cliente, key, 256);
            receive_message(socket_cliente, value1, 256);
            receive_message(socket_cliente, (char*)&N_value2, sizeof(int));
            receive_message(socket_cliente, (char*)&V_value2, N_value2 * sizeof(float));
            // Recibir struct Paquete campo por campo (independiente del lenguaje)
            receive_message(socket_cliente, (char*)&value3.x, sizeof(int));
            receive_message(socket_cliente, (char*)&value3.y, sizeof(int));
            receive_message(socket_cliente, (char*)&value3.z, sizeof(int));

            //despues de recibir todo tengo qe llamar a la funcion
            resultado = modify_value(key, value1, N_value2, V_value2, value3);
            //lo mando
            send_message(socket_cliente, (char *) &resultado, sizeof(int));
            break;
        case 4: //delete
            receive_message(socket_cliente, key, 256);
            resultado = delete_key(key);
            send_message(socket_cliente, (char *) &resultado, sizeof(int));
            break;
        case 5: //exist
            receive_message(socket_cliente, key, 256);
            resultado = exist(key);
            send_message(socket_cliente, (char *) &resultado, sizeof(int));
            break;

        default:
            printf("Error con el numero de la operacion");
            resultado = -1; //para devolver q ha sido un error
            send_message(socket_cliente, (char *) &resultado, sizeof(int));
            break;
    }

    printf("Peticion procesada de manera correcta, el resultado es: %d\n", resultado);

    close(socket_cliente);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
    int socket_servidor;
    int socket_cliente;
    int port;
    pthread_t thread;
    int *cliente_socket_servidor;


    //limpoieza de errores
    if (argc != 2) {
        printf("Error de uso: ./servidor-socket <port>\n");
        return -1;
    }

    port = atoi(argv[1]);

    //priemro tengo q cerar el socket del servidor
    socket_servidor = server_socket(port);
    if (socket_servidor == -1) {printf("Error creando socket\n"); return -1;}

    printf("Escuhando puerto\n");

    //atendenos las peticiones de manera concurrente haciendo uso de HILOS
    while (1) {
        //rpiemro acepto la conexion
        socket_cliente = server_accept(socket_servidor);
        if (socket_cliente == -1) {printf("Error aceptando conexion\n"); continue;}

        //ahora necesito crear hilo
        cliente_socket_servidor = (int *) malloc(sizeof(int));
        *cliente_socket_servidor = socket_cliente;

        pthread_create(&thread, NULL, gestion_cliente, (void*) cliente_socket_servidor);

        //necesito q el hilo sea capaz de liberarse auto
        pthread_detach(thread);
    }
    return 0;
}