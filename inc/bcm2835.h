#ifndef BCM2835_H_
#define BCM2835_H_

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "bcm2835Driver.h"

typedef struct
{
    unsigned int estadoLampada01 : 1;
    unsigned int estadoLampada02 : 1;
    unsigned int estadoLampada03 : 1;
    unsigned int estadoLampada04 : 1;
    unsigned int estadoArCondicionado01 : 1;
    unsigned int estadoArCondicionado02 : 1;
    float temperatura;
    float humidade;
} EstadoDispositivos;

typedef struct {
    unsigned int sensorPresensa01 : 1;
    unsigned int sensorPresensa02 : 1;
    unsigned int sensorAbertura01 : 1;
    unsigned int sensorAbertura02 : 1;
    unsigned int sensorAbertura03 : 1;
    unsigned int sensorAbertura04 : 1;
    unsigned int sensorAbertura05 : 1;
    unsigned int sensorAbertura06 : 1;
} EstadoSensores;

#define LAMPADA_01 RPI_V2_GPIO_P1_11          // Lâmpada Cozinha
#define LAMPADA_02 RPI_V2_GPIO_P1_12          // Lâmpada Sala
#define LAMPADA_03 RPI_V2_GPIO_P1_13          // Lâmpada Quarto 01
#define LAMPADA_04 RPI_V2_GPIO_P1_15          // Lâmpada Quarto 02
#define AR_CONDICIONADO_01 RPI_V2_GPIO_P1_16  // Ar Condicionado Quarto 01
#define AR_CONDICIONADO_02 RPI_V2_GPIO_P1_18  // Ar Condicionado Quarto 02
#define SENSOR_PRESENSA_01 RPI_V2_GPIO_P1_22  // Sensor Presensa Sala
#define SENSOR_PRESENSA_02 RPI_V2_GPIO_P1_37  // Sensor Presensa Cozinha
#define SENSOR_ABERTURA_01 RPI_V2_GPIO_P1_29  // Sensor Abertura Porta Cozinha
#define SENSOR_ABERTURA_02 RPI_V2_GPIO_P1_31  // Sensor Abertura Janela Cozinha
#define SENSOR_ABERTURA_03 RPI_V2_GPIO_P1_32  // Sensor Abertura Porta Sala
#define SENSOR_ABERTURA_04 RPI_V2_GPIO_P1_36  // Sensor Abertura Janela Sala
#define SENSOR_ABERTURA_05 RPI_V2_GPIO_P1_38  // Sensor Abertura Janela Quarto 01
#define SENSOR_ABERTURA_06 RPI_V2_GPIO_P1_40  // Sensor Abertura Janela Quarto 02
#define NUM_SENSORES 8

void bcm2835_acionarDispositivo(int numDispositivo, int estado);
EstadoDispositivos bcm2835_verificaDispositivos(EstadoDispositivos estadoDispositivos);
EstadoSensores bcm2835_verificaSensores(EstadoSensores estadoSensores);
void bcm2835_ligarVentoinha(int ligar);
void bcm2835_inicializa();
void bcm2835_desliga();

#endif /* BCM2835_H_ */
