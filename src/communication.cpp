#include <Arduino.h>
#include "communication.h"


#define PIN_TX 21
#define PIN_RX 22


#define TEMPS_BIT 500



void initialiserCommunication()
{
    pinMode(PIN_TX, OUTPUT);
    pinMode(PIN_RX, INPUT);


    digitalWrite(PIN_TX, LOW);


    Serial.println("Communication initialisee");
}





// Envoie un bit en Manchester
void envoyerBitManchester(uint8_t bit)
{
    if(bit == 0)
    {
        digitalWrite(PIN_TX, HIGH);
        delayMicroseconds(TEMPS_BIT);

        digitalWrite(PIN_TX, LOW);
        delayMicroseconds(TEMPS_BIT);
    }
    else
    {
        digitalWrite(PIN_TX, LOW);
        delayMicroseconds(TEMPS_BIT);

        digitalWrite(PIN_TX, HIGH);
        delayMicroseconds(TEMPS_BIT);
    }
}





// Envoie un octet encodé en Manchester
void envoyerOctetManchester(uint8_t octet)
{
    for(int i = 7; i >= 0; i--)
    {
        uint8_t bit = (octet >> i) & 1;

        envoyerBitManchester(bit);
    }
}





void envoyerTrame(const Trame& trame)
{
    // Ici tu vas :
    // 1- transformer la trame en tableau d'octets
    // 2- encoder Manchester
    // 3- envoyer sur PIN_TX



    // Envoyer le préambule

    envoyerOctetManchester(trame.Preambule);



    // Envoyer le caractère de début

    envoyerOctetManchester(trame.start);



    // Envoyer l'entête

    envoyerOctetManchester((uint8_t)trame.entete.type);

    envoyerOctetManchester(trame.entete.NumeroSequence);

    envoyerOctetManchester(trame.entete.LongueurChargeUtile);

    envoyerOctetManchester(trame.entete.VolumeDynamique);




    // Envoyer la charge utile

    for(uint8_t i = 0; i < trame.entete.LongueurChargeUtile; i++)
    {
        envoyerOctetManchester(trame.ChargeUtile[i]);
    }





    // Envoyer le CRC

    envoyerOctetManchester((uint8_t)(trame.crc >> 8));

    envoyerOctetManchester((uint8_t)(trame.crc & 0xFF));





    // Envoyer le caractère de fin

    envoyerOctetManchester(trame.end);



    Serial.println("Trame envoyee");
}






// Lire un bit Manchester

uint8_t recevoirBitManchester()
{
    uint8_t premier = digitalRead(PIN_RX);


    delayMicroseconds(TEMPS_BIT);


    uint8_t deuxieme = digitalRead(PIN_RX);



    if(premier == HIGH && deuxieme == LOW)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}







// Lire un octet Manchester

uint8_t recevoirOctetManchester()
{
    uint8_t octet = 0;


    for(int i = 7; i >= 0; i--)
    {
        uint8_t bit = recevoirBitManchester();

        octet |= (bit << i);
    }


    return octet;
}






bool recevoirTrame(Trame& trame)
{

    // Ici tu vas :
    // 1- lire PIN_RX
    // 2- décoder Manchester
    // 3- reconstruire la trame





    // Lire le préambule

    trame.Preambule = recevoirOctetManchester();



    // Lire le start

    trame.start = recevoirOctetManchester();




    // Vérifier le début de trame

    if(trame.Preambule != 0x55 ||
       trame.start != 0x7E)
    {
        return false;
    }






    // Lire l'entête

    trame.entete.type =
    (TypeCommunication)recevoirOctetManchester();



    trame.entete.NumeroSequence =
    recevoirOctetManchester();



    trame.entete.LongueurChargeUtile =
    recevoirOctetManchester();



    trame.entete.VolumeDynamique =
    recevoirOctetManchester();






    // Lire la charge utile

    for(uint8_t i = 0;
        i < trame.entete.LongueurChargeUtile;
        i++)
    {
        trame.ChargeUtile[i] =
        recevoirOctetManchester();
    }






    // Lire le CRC

    uint8_t crcHaut = recevoirOctetManchester();

    uint8_t crcBas = recevoirOctetManchester();



    trame.crc =
    ((uint16_t)crcHaut << 8) | crcBas;







    // Lire le caractère de fin

    trame.end = recevoirOctetManchester();





    // Vérification du caractère de fin

    if(trame.end != 0x7E)
    {
        return false;
    }




    Serial.println("Trame recue");


    return true;
}