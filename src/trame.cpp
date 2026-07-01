#include <Arduino.h>
#include "trame.h"


// Constructeur CRC16
CRC16::CRC16()
{
    crc = 0xFFFF;
}


// Traitement d'un octet pour le CRC16
uint16_t CRC16::processByte(uint8_t data)
{
    crc ^= data;

    for (uint8_t i = 0; i < 8; i++)
    {
        if (crc & 1)
        {
            crc = (crc >> 1) ^ 0xA001;
        }
        else
        {
            crc >>= 1;
        }
    }

    return crc;
}


// Traitement d'un tableau d'octets pour le CRC16
uint16_t CRC16::processBuffer(const uint8_t *data_p, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        processByte(data_p[i]);
    }

    return crc;
}



Trame creerTrame(TypeCommunication type,
                 uint8_t numeroSequence,
                 uint8_t longueurChargeUtile,
                 uint8_t volumeDynamique,
                 const uint8_t* chargeUtile)
{
    Trame trame;


    if (longueurChargeUtile > MAX_CHARGE_UTILE)
    {
        longueurChargeUtile = MAX_CHARGE_UTILE;
    }


    trame.Preambule = 0x55;
    trame.start = 0x7E;
    trame.end = 0x7E;


    trame.entete.type = type;
    trame.entete.NumeroSequence = numeroSequence;
    trame.entete.LongueurChargeUtile = longueurChargeUtile;
    trame.entete.VolumeDynamique = volumeDynamique;


    // Copier la charge utile dans la trame
    if (chargeUtile != nullptr)
    {
        for (uint8_t i = 0; i < longueurChargeUtile; i++)
        {
            trame.ChargeUtile[i] = chargeUtile[i];
        }
    }


    // Calcul du CRC16 de la trame
    trame.crc = calculCRC16(trame);


    return trame;
}



uint16_t calculCRC16(const Trame& trame)
{
    CRC16 crc;


    // Calcul du CRC sur l'entête

    crc.processByte((uint8_t)trame.entete.type);
    crc.processByte(trame.entete.NumeroSequence);
    crc.processByte(trame.entete.LongueurChargeUtile);
    crc.processByte(trame.entete.VolumeDynamique);


    // Calcul sur la charge utile

    crc.processBuffer(
        trame.ChargeUtile,
        trame.entete.LongueurChargeUtile
    );


    return crc.getCrc();
}