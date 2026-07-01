#ifndef TRAME_H
#define TRAME_H

#include <Arduino.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_CHARGE_UTILE 80

enum TypeCommunication: uint8_t {
    debut = 0x01,
    data = 0x02,
    fin = 0x03,
    NACK = 0x04
};

struct Entete {
    TypeCommunication type;
    uint8_t NumeroSequence;
    uint8_t LongueurChargeUtile;
    uint8_t VolumeDynamique;
};

struct Trame {
    uint8_t Preambule; //0x55
    uint8_t start; //0x7E
    Entete entete;
    uint8_t ChargeUtile[MAX_CHARGE_UTILE];  //0 à 80 octets
    uint16_t crc;
    uint8_t end; //0x7E
};

class CRC16 {
	public:
	   CRC16();
	   uint16_t processByte(uint8_t data);
	   uint16_t processBuffer(const uint8_t *data_p, uint16_t length);
	   uint16_t getCrc() { return crc; };
	private:
	   uint16_t crc;
};	

// Fonctions de création/calcul
Trame creerTrame(TypeCommunication type,
                 uint8_t numeroSequence,
                 uint8_t longueurChargeUtile,
                 uint8_t volumeDynamique,
                 const uint8_t* chargeUtile);


uint16_t calculCRC16(const Trame& trame);

#endif // TRAME_H