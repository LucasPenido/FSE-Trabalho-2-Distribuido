#include "controle.h"

void *informaEstadoDispositivos() {
    EstadoDispositivos estadoDispositivos = {0, 0, 0, 0, 0, 0};

    TipoConexao tipoConexao;
    int tamMensagem;

    tipoConexao.tipoConexao = CONEXAO_TIPO_DISPOSITIVO;
    tamMensagem = sizeof(tipoConexao) + sizeof(bme280TemperaturaUmidade) + sizeof(estadoDispositivos);

    while (1) {
        sleep(1);
        bme280_defineTemperaturaUmidade();
        estadoDispositivos = bcm2835_verificaDispositivos();

        uint8_t *pacoteEnvio = calloc(tamMensagem, sizeof(uint8_t));
        uint8_t *pacote = (uint8_t *)pacoteEnvio;

        memcpy(pacote, &tipoConexao, sizeof(tipoConexao));
        pacote += sizeof(tipoConexao);

        memcpy(pacote, &estadoDispositivos, sizeof(estadoDispositivos));
        pacote += sizeof(estadoDispositivos);

        memcpy(pacote, &bme280TemperaturaUmidade, sizeof(bme280TemperaturaUmidade));
        pacote += sizeof(bme280TemperaturaUmidade);

        if (enviaMensagemServidorCentral(pacoteEnvio, tamMensagem) == 0) {
            free(pacoteEnvio);
            break;
        }
        free(pacoteEnvio);
    }

    return NULL;
}

int informaEstadoSensores(EstadoSensores estadoSensores) {
    TipoConexao tipoConexao;
    int tamMensagem;

    tipoConexao.tipoConexao = CONEXAO_TIPO_SENSOR;
    tamMensagem = sizeof(estadoSensores) + sizeof(tipoConexao);

    uint8_t *pacoteEnvio = calloc(tamMensagem, sizeof(uint8_t));
    uint8_t *pacote = (uint8_t *)pacoteEnvio;

    memcpy(pacote, &tipoConexao, sizeof(tipoConexao));
    pacote += sizeof(tipoConexao);

    memcpy(pacote, &estadoSensores, sizeof(estadoSensores));
    pacote += sizeof(estadoSensores);

    if (enviaMensagemServidorCentral(pacoteEnvio, tamMensagem) == 0) {
        free(pacoteEnvio);
        return 0;
    }

    free(pacoteEnvio);
    return 1;
}

void *verificaSensores() {
    EstadoSensores estadoSensores = {0, 0, 0, 0, 0, 0, 0, 0};
    int envia = 1;

    while (1) {
        estadoSensores = bcm2835_verificaSensores();
        if (envia) {
            if(informaEstadoSensores(estadoSensores) == 0) {
                break;
            }
        }
        if (estadoSensores.sensorPresensa01 || estadoSensores.sensorPresensa02 ||
            estadoSensores.sensorAbertura01 || estadoSensores.sensorAbertura02 ||
            estadoSensores.sensorAbertura03 || estadoSensores.sensorAbertura04 ||
            estadoSensores.sensorAbertura05 || estadoSensores.sensorAbertura01) {
            envia = 1;
        } else {
            envia = 0;
        }
        usleep(200000);
    }

    return NULL;
}