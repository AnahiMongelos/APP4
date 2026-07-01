#include <Arduino.h>
#include "recepteur.h"
#include "communication.h"


static RecepteurState etatRecepteur = InitialisationRecepteur;

static uint8_t numeroSequenceAttendu = 1;


// Tableau pour stocker les données reçues
static char donneesRecues[5][80];


void updateRecepteur() {

    switch (etatRecepteur) {


        case InitialisationRecepteur:
        {
            // Réception de la trame de début

            Trame trameDebut;

            recevoirTrame(trameDebut);


            if(trameDebut.entete.type == TypeCommunication::debut)
            {
                // Initialisation du compteur de séquence

                numeroSequenceAttendu = 1;

                etatRecepteur = FluxContinuRecepteur;
            }

            break;
        }



        case FluxContinuRecepteur:
        {
            // Code pour le flux continu du récepteur

            Trame trameData;

            recevoirTrame(trameData);


            if(trameData.entete.type == TypeCommunication::data)
            {

                if(trameData.entete.NumeroSequence == numeroSequenceAttendu)
                {
                    // Traiter la trame reçue


                    for(uint8_t i = 0; i < trameData.entete.LongueurChargeUtile; i++)
                    {
                        donneesRecues[numeroSequenceAttendu - 1][i] =
                        trameData.ChargeUtile[i];
                    }


                    donneesRecues[numeroSequenceAttendu - 1]
                    [trameData.entete.LongueurChargeUtile] = '\0';


                    Serial.println("Donnee recue :");

                    Serial.println(
                        donneesRecues[numeroSequenceAttendu - 1]
                    );


                    // Incrémenter le numéro de séquence

                    numeroSequenceAttendu++;

                }
                else
                {
                    // Numéro de séquence inattendu, demander retransmission

                    etatRecepteur = DetectionPanne;
                }

            }


            else if(trameData.entete.type == TypeCommunication::fin)
            {
                etatRecepteur = FermetureRecepteur;
            }


            break;
        }




        case DetectionPanne:
        {
            // Code pour la détection de panne

            Serial.println("Panne détectée");


            etatRecepteur = DemandeRetransmission;


            break;
        }




        case DemandeRetransmission:
        {
            // Code pour la demande de retransmission


            Trame trameNACK = creerTrame(
                TypeCommunication::NACK,
                numeroSequenceAttendu,
                0,
                0,
                nullptr
            );


            envoyerTrame(trameNACK);


            //Vider tampon matériel


            etatRecepteur = FluxContinuRecepteur;


            break;
        }




        case FermetureRecepteur:
        {
            // Code pour la fermeture du récepteur

            Serial.println("Transmission terminee");


            numeroSequenceAttendu = 1;


            etatRecepteur = InitialisationRecepteur;


            break;
        }

    }
}