#include <Arduino.h>
#include "emetteur.h"
#include "communication.h"
#include "acquisition.h"


static EmetteurState etatEmetteur = InitialisationEmetteur;


static uint8_t numeroSequence = 0;

static uint8_t totalPaquets = 0;

static uint8_t numeroSequenceManquant = 0;



void updateEmetteur() {


    switch (etatEmetteur) {



        case InitialisationEmetteur:
        {
            //Transmission de la trame de début


            totalPaquets = obtenirNombrePaquets();


            Trame trameDebut = creerTrame(
                TypeCommunication::debut,
                0,
                0,
                totalPaquets,
                nullptr
            );


            envoyerTrame(trameDebut);


            numeroSequence = 1;


            etatEmetteur = FluxContinuEmetteur;


            break;
        }




        case FluxContinuEmetteur:
        {
            //Transmission des données de manière séquentielle


            const char* donnees = obtenirDonnee(numeroSequence - 1);



            Trame trameData = creerTrame(
                TypeCommunication::data,
                numeroSequence,
                strlen(donnees),
                0,
                (const uint8_t*)donnees
            );


            envoyerTrame(trameData);



            //Incrémenter numéro de séquence

            numeroSequence++;



            if(numeroSequence > totalPaquets)
            {
                etatEmetteur = FermetureEmetteur;
            }
            else
            {
                etatEmetteur = FluxContinuEmetteur;
            }


            //si reception d'un NACK, passer à l'état ReceptionNackEmetteur


            break;
        }





        case ReceptionNackEmetteur:
        {
            //Réception d'un NACK


            //Vider tampon matériel


            numeroSequence = numeroSequenceManquant;


            etatEmetteur = FluxContinuEmetteur;


            break;
        }






        case FermetureEmetteur:
        {
            //Transmission de la trame de fin


            Trame trameFin = creerTrame(
                TypeCommunication::fin,
                numeroSequence,
                0,
                0,
                nullptr
            );


            envoyerTrame(trameFin);



            etatEmetteur = InitialisationEmetteur;


            break;
        }

    }
}




void recevoirNack(uint8_t numeroManquant)
{
    numeroSequenceManquant = numeroManquant;


    etatEmetteur = ReceptionNackEmetteur;
}