#ifndef RECEPTEUR_H
#define RECEPTEUR_H

#include <Arduino.h>
#include "communication.h"


// Définition des états du récepteur
enum RecepteurState {
    InitialisationRecepteur,
    FluxContinuRecepteur,
    DetectionPanne,
    DemandeRetransmission,
    FermetureRecepteur
};

void updateRecepteur();

#endif // RECEPTEUR_H
 