#include <Arduino.h>

#include "emetteur.h"
#include "recepteur.h"
#include "communication.h"
#include "acquisition.h"



void setup()
{
    Serial.begin(9600);


    // Initialisation communication
    initialiserCommunication();


    // Initialisation acquisition des données
    initialiserAcquisition();

}



void loop()
{
    // Mise à jour de l'émetteur
    updateEmetteur();


    // Mise à jour du récepteur
    updateRecepteur();

}