#ifndef ACQUISITION_H
#define ACQUISITION_H

#include <Arduino.h>

#define TOTAL_PAQUETS 5


const char* obtenirDonnee(uint8_t index);

uint8_t obtenirNombrePaquets();


void initialiserAcquisition();


#endif