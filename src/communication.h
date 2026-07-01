#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include "trame.h"

void envoyerTrame(const Trame& trame);

bool recevoirTrame(Trame& trame);

void initialiserCommunication();

// Encodage Manchester
void envoyerOctetManchester(uint8_t octet);


// Décodage Manchester
uint8_t recevoirOctetManchester();

#endif // COMMUNICATION_H