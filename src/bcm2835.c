#include "bcm2835.h"

void configura_pinos() {
    bcm2835_gpio_fsel(LAMPADA_01, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMPADA_02, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMPADA_03, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMPADA_04, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(AR_CONDICIONADO_01, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(AR_CONDICIONADO_02, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(SENSOR_PRESENSA_01, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_PRESENSA_02, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABERTURA_01, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABERTURA_02, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABERTURA_03, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABERTURA_04, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABERTURA_05, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABERTURA_06, BCM2835_GPIO_FSEL_INPT);
}

void bcm2835_acionarDispositivo(int numDispositivo, int estado) {
    bcm2835_gpio_write(numDispositivo, estado);
}

EstadoDispositivos bcm2835_verificaDispositivos(EstadoDispositivos estadoDispositivos){
    printf("%d", bcm2835_gpio_lev(LAMPADA_01));

    return estadoDispositivos;
}

EstadoSensores bcm2835_verificaSensores(EstadoSensores estadoSensores) {
    estadoSensores.sensorPresensa01 = bcm2835_gpio_lev(SENSOR_PRESENSA_01);
    estadoSensores.sensorPresensa02 = bcm2835_gpio_lev(SENSOR_PRESENSA_02);
    estadoSensores.sensorAbertura01 = bcm2835_gpio_lev(SENSOR_ABERTURA_01);
    estadoSensores.sensorAbertura02 = bcm2835_gpio_lev(SENSOR_ABERTURA_02);
    estadoSensores.sensorAbertura03 = bcm2835_gpio_lev(SENSOR_ABERTURA_03);
    estadoSensores.sensorAbertura04 = bcm2835_gpio_lev(SENSOR_ABERTURA_04);
    estadoSensores.sensorAbertura05 = bcm2835_gpio_lev(SENSOR_ABERTURA_05);
    estadoSensores.sensorAbertura06 = bcm2835_gpio_lev(SENSOR_ABERTURA_06);

    
    return estadoSensores;
}

void bcm2835_desliga() {
    bcm2835_close();
}

void bcm2835_inicializa() {
    const struct sched_param priority = {1};
    sched_setscheduler(0, SCHED_FIFO, &priority);

    mlockall(MCL_CURRENT | MCL_FUTURE);
    if (!bcm2835_init()) {
        exit(1);
    }

    configura_pinos();
}