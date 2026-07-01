#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include "trame.h"

void envoyerTrame(const Trame& trame);

bool recevoirTrame(Trame& trame);

void initialiserCommunication();

#endif // COMMUNICATION_H