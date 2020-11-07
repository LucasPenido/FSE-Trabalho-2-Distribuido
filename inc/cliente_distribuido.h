#ifndef CLIENTE_DISTRIBUIDO_H
#define CLIENTE_DISTRIBUIDO_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "controle.h"

#define SERV_CENTRAL_ADDR "192.168.0.53"
#define SERV_CENTRAL_TCP_PORT 10014

#define CONEXAO_TIPO_DISPOSITIVO 0
#define CONEXAO_TIPO_SENSOR 1

extern int socketServCentralfd;

typedef struct {
    unsigned int tipoConexao : 1;  // 0 - Dispositivos; 1 - EstadoSensores;
} TipoConexao;

int enviaMensagemServidorCentral(uint8_t *pacoteEnvio, int tamMensagem);
void *mantemConexaoServidorCentral();

#endif