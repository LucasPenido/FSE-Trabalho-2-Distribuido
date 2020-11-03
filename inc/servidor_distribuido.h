#ifndef SERVIDOR_DISTRIBUIDO_H
#define SERVIDOR_DISTRIBUIDO_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "bcm2835.h"

#define SERV_DISTRIBUIDO_TCP_PORT 10114

extern int socketServDistribuidofd;
extern int clienteSocketfd;

typedef struct
{
    unsigned int estado : 1;  // Ligado / Desligado
    int numDispositivo;
} MensagemDispositivo;

void *realizaConexaoClienteCentral();
void criaServidorDistribuido();

#endif