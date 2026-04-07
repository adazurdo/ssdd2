//
// Created by adazu on 07/04/2026.
//

#include "common.h"

#include <unistd.h>
#include <stdio.h>

int send_message(int socket, char *buffer, int len) {
    int r;
    int l = len;

    while (l > 0) {
        r = write(socket, buffer, l);
        if (r < 0) {
            printf("Error al enviar mensaje\n");
            return -1;
        }
        l = l - r;
        buffer += r;
    }
    return 0;
}

int receive_message(int socket, char *buffer, int len) {
    int r;
    int l = len;

    while (l > 0) {
        r = read(socket, buffer, l);
        if (r < 0) {
            printf("Error al recibir mensaje\n");
            return -1;
        }
        l = l - r;
        buffer += r;
    }
    return 0;
}