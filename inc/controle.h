#ifndef CONTROLE_H
#define CONTROLE_H

#include "cliente_distribuido.h"
#include "bcm2835.h"
#include "bme280.h"

void *informaEstadoDispositivos();
void *verificaSensores();

#endif
