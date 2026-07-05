#include <Arduino.h>

#include "communication.h"
#include "acquisition.h"
#include "emetteur.h"
#include "recepteur.h"


void TacheEmetteur(void *pvParameters)
{
    while(true)
    {
        updateEmetteur();

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


void TacheRecepteur(void *pvParameters)
{
    while(true)
    {
        updateRecepteur();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}


void setup()
{
    Serial.begin(9600);

    initialiserCommunication();

    initialiserAcquisition();

    xTaskCreatePinnedToCore(
        TacheEmetteur,
        "Emetteur",
        4096,
        NULL,
        1,
        NULL,
        1
    );

    xTaskCreatePinnedToCore(
        TacheRecepteur,
        "Recepteur",
        4096,
        NULL,
        1,
        NULL,
        0
    );
}


void loop()
{
    vTaskDelete(NULL);
}