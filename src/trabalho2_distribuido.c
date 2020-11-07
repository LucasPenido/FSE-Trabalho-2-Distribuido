#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "bcm2835.h"
#include "bme280.h"
#include "cliente_distribuido.h"
#include "servidor_distribuido.h"

#define MAXLINE 512

volatile int tempoUmSegundo = 0;
volatile float temperaturaExterna = 0;

void fechaConexoes() {
    bcm2835_desliga();
    close(socketServCentralfd);
    close(socketServDistribuidofd);
    close(clienteSocketfd);
}

void trata_interrupcao() {
    exit(1);
}

void trata_pipe() {
    printf("Servidor Central Desconectado\n");
    exit(1);
}

int main(int argc, char const *argv[]) {
    pthread_t thread_cliente, thread_servidor;

    atexit(fechaConexoes);
    signal(SIGINT, trata_interrupcao);
    signal(SIGPIPE, trata_pipe);

    bme280_inicializa();
    bcm2835_inicializa();

    criaServidorDistribuido();

    // // bcm2835_verificaDispositivos(estadoDispositivos);

    pthread_create(&thread_servidor, NULL, &realizaConexaoClienteCentral, NULL);
    pthread_create(&thread_cliente, NULL, &mantemConexaoServidorCentral, NULL);
    pthread_join(thread_cliente, NULL);
    pthread_join(thread_servidor, NULL);

    fechaConexoes();

    return 0;
}
