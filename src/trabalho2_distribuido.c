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
EstadoSensores estadoSensores = {0, 0, 0, 0, 0, 0, 0, 0};
EstadoDispositivos estadoDispositivos = {0, 0, 0, 0, 0, 0, 0, 0};

pthread_mutex_t verificaSensoresMutex;
pthread_cond_t verificaSensoresCond;

void fechaConexoes() {
    bcm2835_desliga();
    close(socketServCentralfd);
    close(socketServDistribuidofd);
    close(clienteSocketfd);
}

void trata_interrupcao() {
    exit(1);
}

void trata_alarme() {
    ualarm(200000, 0);

    pthread_cond_signal(&verificaSensoresCond);

    if (tempoUmSegundo == 1000) {
        tempoUmSegundo = 0;
    }

    tempoUmSegundo += 200;
}

// void *lerTemperaturaBme280() {
//     float temp;
//     while (1) {
//         // pthread_mutex_lock(&lerTemperaturaBme280Mutex);
//         // pthread_cond_wait(&lerTemperaturaBme280Cond, &lerTemperaturaBme280Mutex);
//         if ((temp = bme280_requisitaTemperaturaExterna()) > 0) {
//             temperaturaExterna = temp;
//         }
//         // pthread_mutex_unlock(&lerTemperaturaBme280Mutex);
//     }
// }

void informaEstadoDispositivos() {
}

void informarAcionamentoSensor(EstadoSensores estadoSensores) {
    TipoConexao tipoConexao;
    int tamMensagem;

    tipoConexao.tipoConexao = CONEXAO_TIPO_SENSOR;

    uint8_t *pacoteEnvio = calloc(sizeof(estadoSensores) + sizeof(tipoConexao), sizeof(uint8_t));
    uint8_t *pacote = (uint8_t *)pacoteEnvio;

    memcpy(pacote, &tipoConexao, sizeof(tipoConexao));
    pacote += sizeof(tipoConexao);

    memcpy(pacote, &estadoSensores, sizeof(estadoSensores));
    pacote += sizeof(estadoSensores);

    tamMensagem = sizeof(estadoSensores) + sizeof(tipoConexao);

    enviaMensagemServidorCentral(pacoteEnvio, tamMensagem);
}

void *verificaSensores() {
    while (1) {
        pthread_mutex_lock(&verificaSensoresMutex);
        pthread_cond_wait(&verificaSensoresCond, &verificaSensoresMutex);
        estadoSensores = bcm2835_verificaSensores(estadoSensores);
        if (estadoSensores.sensorPresensa01 || estadoSensores.sensorPresensa02 ||
            estadoSensores.sensorAbertura01 || estadoSensores.sensorAbertura02 ||
            estadoSensores.sensorAbertura03 || estadoSensores.sensorAbertura04 ||
            estadoSensores.sensorAbertura05 || estadoSensores.sensorAbertura01) {
            informarAcionamentoSensor(estadoSensores);
        } else {
            //
        }
        pthread_mutex_unlock(&verificaSensoresMutex);
    }
}

int main(int argc, char const *argv[]) {
    pthread_t thread_cliente, thread_verifica_sensores, thread_servidor;

    atexit(fechaConexoes);
    signal(SIGINT, trata_interrupcao);
    signal(SIGALRM, trata_alarme);

    ualarm(200000, 0);
    bme280_inicializa();
    bme280_requisitaTemperaturaUmidade();
    bcm2835_inicializa();

    // bcm2835_verificaDispositivos(estadoDispositivos);
    conectarServidorCentral();
    criaServidorDistribuido();
    // printf("%d\n", estadoSensores[0]);
    // unsigned char buffer[16];

    // buffer[0] = 0b11011100;
    // printf("%d\n", (0b1011100 >> 6) & 1);
    // printf("%d\n", buffer[0]);

    // int result = (1 << 1) | 1;
    // result = (13 << 1) | 0;
    // pthread_create(&thread_temperatura, NULL, &lerTemperaturaBme280, NULL);
    pthread_create(&thread_servidor, NULL, &realizaConexaoClienteCentral, NULL);
    pthread_create(&thread_verifica_sensores, NULL, &verificaSensores, NULL);
    pthread_join(thread_verifica_sensores, NULL);
    // pthread_join(thread_cliente, NULL);
    // pthread_join(thread_servidor, NULL);

    fechaConexoes();

    return 0;
}
