/*
  Laboratoire : Interruption
  Connexion : Relier le GPIO 12 (Générateur) au 
              GPIO 14 (Interruption) avec un fil.
*/

#include <Arduino.h>

// Définition des broches
const int PIN_GENERATEUR = 12;   // Broche qui va simuler l'événement physique
const int PIN_INTERRUPTION = 14; // Broche qui va intercepter l'interruption
const int PIN_DEL = 2;           // DEL intégrée pour visualiser le résultat

volatile bool drapeauInterruption = false;
volatile int compteurInterruptions = 0;

// --- FONCTION DE GESTION DE L'INTERRUPTION (ISR) ---
void IRAM_ATTR gestionnaireSignal() {
  drapeauInterruption = true;
  compteurInterruptions++;
}

// Tâche FreeRTOS pour générer le signal de manière asynchrone (Core 0)
void TacheGeneratriceSignal(void * pvParameters) {
  while(1) {
    // Génération d'une impulsion (Front descendant : HAUT -> BAS)
    digitalWrite(PIN_GENERATEUR, HIGH);
    vTaskDelay(pdMS_TO_TICKS(10)); // Maintien de l'état haut
    
    Serial.println("\n[GENERATEUR] Action électrique : Force le GPIO 12 à l'état BAS...");
    digitalWrite(PIN_GENERATEUR, LOW); 
    
    // Attente aléatoire entre 1.5 et 4 secondes avant la prochaine impulsion
    int prochainDelai = random(1500, 4000);
    vTaskDelay(pdMS_TO_TICKS(prochainDelai));
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  delay(1000);

  Serial.println("\n==================================================");
  Serial.println("  TEST INTERRUPTION ESP32 EN LOOP-BACK MATÉRIEL   ");
  Serial.println("==================================================");

  // 1. Configuration de la DEL et du Générateur
  pinMode(PIN_DEL, OUTPUT);
  digitalWrite(PIN_DEL, LOW);
  
  pinMode(PIN_GENERATEUR, OUTPUT);
  digitalWrite(PIN_GENERATEUR, HIGH); // État initial haut

  // 2. Configuration de l'entrée d'interruption
  pinMode(PIN_INTERRUPTION, INPUT); 

  // 3. Attachement de l'interruption (Front descendant)
  attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPTION), gestionnaireSignal, FALLING);

  // 4. Création de la tâche de génération sur le Core 0
  xTaskCreatePinnedToCore(TacheGeneratriceSignal, "Generateur", 2048, NULL, 1, NULL, 0);

  Serial.println("Système prêt. En attente des impulsions électriques sur le fil...");
}

void loop() {
  // Le code principal (Core 1) scrute le drapeau levé par l'ISR (Core 0)
  if (drapeauInterruption) {
    
    // Inversion de l'état de la DEL intégrée
    digitalWrite(PIN_DEL, !digitalRead(PIN_DEL));
    
    Serial.print("[MAIN LOOP] Interruption validée en RAM ! Total d'événements : ");
    Serial.println(compteurInterruptions);
    
    // Remise à zéro du drapeau
    drapeauInterruption = false;
  }
  
  vTaskDelay(pdMS_TO_TICKS(10));
}
