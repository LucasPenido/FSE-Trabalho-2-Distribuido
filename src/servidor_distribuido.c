

#include "servidor_distribuido.h"

int socketServDistribuidofd, clienteSocketfd;

void TrataClienteCentral(int sockfd) {
    int n;
    char line[512];
    MensagemDispositivo *mensagemDispositivo;
    while (1) {
        bzero(line, 512);
        n = recv(sockfd, line, 511, 0);
        if (n < 0) {
            printf("erro em ler do socket\n");
        } else if (n == 0) {
            return;
        } else {
            mensagemDispositivo = (MensagemDispositivo *)line;
            bcm2835_acionarDispositivo(mensagemDispositivo->numDispositivo, mensagemDispositivo->estado);
        }
    }
}

void *realizaConexaoClienteCentral() {
    unsigned int cliLen;
    struct sockaddr_in clienteAddr;

    while (1) {
        cliLen = sizeof(clienteAddr);
        clienteSocketfd = accept(socketServDistribuidofd, (struct sockaddr *)&clienteAddr, &cliLen);
        if (clienteSocketfd < 0) {
            printf("Erro ao conectar com Servidor Central\n");
        } else {
            system("clear");
            printf("Conectado ao Servidor Central\n");
            TrataClienteCentral(clienteSocketfd);
        }
        close(clienteSocketfd);
    }
}

void criaServidorDistribuido() {
    struct sockaddr_in servaddr;

    socketServDistribuidofd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServDistribuidofd < 0) {
        printf("ERROR opening socket");
        exit(1);
    }

    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_DISTRIBUIDO_TCP_PORT);

    if (bind(socketServDistribuidofd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("Erro biding %s\n", strerror(errno));
        exit(1);
    }
    if (listen(socketServDistribuidofd, 10) < 0) {
        printf("Erro ao criar listen\n");
        exit(1);
    }
}
