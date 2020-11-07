#include "cliente_distribuido.h"

int socketServCentralfd;

int enviaMensagemServidorCentral(uint8_t *pacoteEnvio, int tamMensagem) {
    if (send(socketServCentralfd, pacoteEnvio, tamMensagem, 0) != tamMensagem) {
        return 0;
    }
    return 1;
}

void *mantemConexaoServidorCentral() {
    pthread_t thread_verifica_dispositivos, thread_verifica_sensores;

    struct sockaddr_in servaddr;

    socketServCentralfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServCentralfd < 0) {
        printf("Erro ao abrir socket\n");
        exit(1);
    }

    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERV_CENTRAL_ADDR);
    servaddr.sin_port = htons(SERV_CENTRAL_TCP_PORT);

    printf("Aguardando Servidor Central estar disponível\n");
    while (1) {
        if (connect(socketServCentralfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
            system("clear");
            printf("Conectado ao Servidor Central\n");
            pthread_create(&thread_verifica_dispositivos, NULL, &informaEstadoDispositivos, NULL);
            pthread_create(&thread_verifica_sensores, NULL, &verificaSensores, NULL);
            pthread_join(thread_verifica_dispositivos, NULL);
            pthread_join(thread_verifica_sensores, NULL);
        }
        usleep(500000);
    }
}