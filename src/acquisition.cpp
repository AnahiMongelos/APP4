#include "acquisition.h"


const char* donnees_txt[TOTAL_PAQUETS] = {

    "Ligne 1 - 2026-06-17 10:00:01 | Temp: 22.4 C | Humidite: 45.2 % | Node: 01",

    "Ligne 2 - 2026-06-17 10:05:01 | Temp: 22.5 C | Humidite: 45.1 % | Node: 01",

    "Ligne 3 - 2026-06-17 10:10:01 | Temp: 22.8 C | Humidite: 44.9 % | Node: 01",

    "Ligne 4 - 2026-06-17 10:15:01 | Temp: 23.1 C | Humidite: 44.8 % | Node: 01",

    "Ligne 5 - 2026-06-17 10:20:01 | Temp: 23.0 C | Humidite: 45.0 % | Node: 01"

};


void initialiserAcquisition()
{
    Serial.println("Acquisition initialisee");
}


const char* obtenirDonnee(uint8_t index)
{
    if(index < TOTAL_PAQUETS)
    {
        return donnees_txt[index];
    }

    return nullptr;
}


uint8_t obtenirNombrePaquets()
{
    return TOTAL_PAQUETS;
}