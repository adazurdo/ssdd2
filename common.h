//
// Created by adazu on 07/04/2026.
//

#ifndef DISTRIBUIDOS_COMMON_H
#define DISTRIBUIDOS_COMMON_H

int send_message(int socket, char *buffer, int len);
int receive_message(int socket, char *buffer, int len);


#endif //DISTRIBUIDOS_COMMON_H