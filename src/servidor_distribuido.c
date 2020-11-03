

#include "servidor_distribuido.h"

int socketServDistribuidofd, clienteSocketfd;

void TrataClienteCentral(int sockfd) {
    int n;
    char line[512];
    MensagemDispositivo *mensagemDispositivo;
    for (;;) {
        bzero(line, 512);
        printf("---%d\n", sockfd);
        n = recv(sockfd, line, 511, 0);
        if (n == 0) {
            printf("recebeu1\n");
            return; /* termina a conexão */

        } else if (n < 0) {
            printf("erro em ler do socket\n");
        } else {
            printf("recebeu3\n");
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
        printf("Aguardando Cliente Central\n");
        clienteSocketfd = accept(socketServDistribuidofd, (struct sockaddr *)&clienteAddr, &cliLen);
        if (clienteSocketfd < 0) {
            printf("ERROR on accept\n");
        } else {
            printf("conectou\n");
        }
        TrataClienteCentral(clienteSocketfd);
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
        printf("Erro biding\n");
        exit(1);
    }
    if (listen(socketServDistribuidofd, 10) < 0) {
        printf("Erro ao criar listen\n");
        exit(1);
    }
}
