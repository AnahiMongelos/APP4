#ifndef EMETTEUR_H
#define EMETTEUR_H

#include <Arduino.h>
#include "communication.h"

//états de l'émetteur
enum EmetteurState: uint8_t {
    InitialisationEmetteur,
    FluxContinuEmetteur,
    ReceptionNackEmetteur,
    FermetureEmetteur
};

void updateEmetteur();

#endif // EMETTEUR_H