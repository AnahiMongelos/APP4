#include <Arduino.h>
#include "communication.h"


#define PIN_TX 21
#define PIN_RX 22


void initialiserCommunication()
{
    // Initialisation communication série pour le debug
    Serial.begin(9600);


    // Initialisation communication entre ESP32
    Serial1.begin(
        9600,
        SERIAL_8N1,
        PIN_RX,
        PIN_TX
    );


    Serial.println("Communication initialisee");
}



void envoyerTrame(const Trame& trame)
{
    // Envoyer le préambule
    Serial1.write(trame.Preambule);


    // Envoyer le caractère de début
    Serial1.write(trame.start);



    // Envoyer l'entête

    Serial1.write((uint8_t)trame.entete.type);

    Serial1.write(trame.entete.NumeroSequence);

    Serial1.write(trame.entete.LongueurChargeUtile);

    Serial1.write(trame.entete.VolumeDynamique);



    // Envoyer la charge utile

    for(uint8_t i = 0; i < trame.entete.LongueurChargeUtile; i++)
    {
        Serial1.write(trame.ChargeUtile[i]);
    }



    // Envoyer le CRC

    Serial1.write((uint8_t)(trame.crc >> 8));

    Serial1.write((uint8_t)(trame.crc & 0xFF));



    // Envoyer le caractère de fin

    Serial1.write(trame.end);



    // Message de debug seulement sur USB

    Serial.println("Trame envoyee");
}





bool recevoirTrame(Trame& trame)
{

    // Vérifier si une donnée est disponible

    if(Serial1.available() < 2)
    {
        return false;
    }



    // Lire le préambule

    trame.Preambule = Serial1.read();



    // Lire le start

    trame.start = Serial1.read();



    // Vérifier que la trame commence correctement

    if(trame.Preambule != 0x55 ||
       trame.start != 0x7E)
    {
        return false;
    }



    // Lire l'entête

    while(Serial1.available() < 4)
    {
        return false;
    }


    trame.entete.type =
    (TypeCommunication)Serial1.read();


    trame.entete.NumeroSequence =
    Serial1.read();


    trame.entete.LongueurChargeUtile =
    Serial1.read();


    trame.entete.VolumeDynamique =
    Serial1.read();




    // Lire la charge utile

    for(uint8_t i = 0;
        i < trame.entete.LongueurChargeUtile;
        i++)
    {
        while(!Serial1.available());

        trame.ChargeUtile[i] = Serial1.read();
    }



    // Lire le CRC

    while(Serial1.available() < 2);


    trame.crc = ((uint16_t)Serial1.read() << 8);

    trame.crc |= Serial1.read();



    // Lire la fin

    while(!Serial1.available());

    trame.end = Serial1.read();



    Serial.println("Trame recue");


    return true;
}