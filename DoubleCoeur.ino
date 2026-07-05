/*
  Exploration de l'architecture Double Coeur
  Objectif : Lancer des threads spécifiques sur le Core 0 et le Core 1
*/
#include <Arduino.h>
// Déclaration des prototypes de fonctions pour les tâches
void TacheSurCore0(void * pvParameters);
void TacheSurCore1(void * pvParameters);

void setup() {
  // Initialisation du port série pour observer les résultats
  Serial.begin(115200);
  while(!Serial); // Attend que le moniteur série soit ouvert
  
  delay(1000);
  Serial.println("\n==================================================");
  Serial.println("  DÉBUT DE L'EXPLORATION DOUBLE COEUR DE L'ESP32  ");
  Serial.println("==================================================");

  // Par défaut, le setup() d'Arduino s'exécute sur le CORE 1.
  Serial.print("[SETUP] La fonction setup() s'exécute sur le CORE : ");
  Serial.println(xPortGetCoreID());

  // =================================================================
  // 1. LANCEMENT DES THREADS SUR LE CORE 0 (PRO_CPU)
  // =================================================================
  Serial.println("\n[SETUP] Création des tâches pour le CORE 0...");
  
  xTaskCreatePinnedToCore(
    TacheSurCore0,     // Fonction qui contient le code de la tâche
    "Thread_0_A",      // Nom textuel de la tâche (pour le débogage)
    2048,              // Taille de la pile (Stack size en octets)
    (void*)"Thread 0A",// Paramètre passé à la fonction
    1,                 // Priorité de la tâche
    NULL,              // Handle de la tâche (pas besoin ici)
    0                  // <--- ID du cœur : 0
  );

  xTaskCreatePinnedToCore(
    TacheSurCore0,
    "Thread_0_B",
    2048,
    (void*)"Thread 0B",
    1,
    NULL,
    0                  // <--- ID du coeur : 0
  );

  // =================================================================
  // 2. LANCEMENT DES THREADS SUR LE CORE 1 (APP_CPU)
  // =================================================================
  Serial.println("[SETUP] Création des tâches pour le CORE 1...");

  xTaskCreatePinnedToCore(
    TacheSurCore1,
    "Thread_1_A",
    2048,
    (void*)"Thread 1A",
    1,
    NULL,
    1                  // <--- ID du coeur : 1
  );

  xTaskCreatePinnedToCore(
    TacheSurCore1,
    "Thread_1_B",
    2048,
    (void*)"Thread 1B",
    1,
    NULL,
    1                  // <--- ID du coeur : 1
  );
}

// --- DÉFINITION DES FONCTIONS DE TÂCHES ---

// Cette fonction sera exécutée par les threads épinglés au CORE 0
void TacheSurCore0(void * pvParameters) {
  char* nomTache = (char*) pvParameters;
  int compteurLocal = 0;

  while(1) {
    compteurLocal++;
    
    // Affichage des informations
    Serial.print("[CORE 0] ");
    Serial.print(nomTache);
    Serial.print(" | Itération : ");
    Serial.println(compteurLocal);

    // vTaskDelay force un changement de contexte pour laisser l'autre thread du Core 0 s'exprimer
    vTaskDelay(pdMS_TO_TICKS(800)); 
  }
}

// Cette fonction sera exécutée par les threads épinglés au CORE 1
void TacheSurCore1(void * pvParameters) {
  char* nomTache = (char*) pvParameters;
  int compteurLocal = 0;

  while(1) {
    compteurLocal++;

    Serial.print("[CORE 1] ───> ");
    Serial.print(nomTache);
    Serial.print(" | Itération : ");
    Serial.println(compteurLocal);

    vTaskDelay(pdMS_TO_TICKS(1200)); // Délai différent pour désynchroniser les affichages
  }
}

void loop() {
  // Nous détruisons la tâche loop par défaut pour libérer les ressources du Core 1.
  vTaskDelete(NULL);
}
