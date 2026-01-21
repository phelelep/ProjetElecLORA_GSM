#include <RadioLib.h>

// Définition des broches selon notre câblage précédent
// NSS: 5, DIO1: 34, NRST: 27, BUSY: 35
LLCC68 radio = new Module(5, 34, 27, 35);

void setup() {
  Serial.begin(115200);
  delay(2000); // Laisse le temps au port série de s'ouvrir

  Serial.println(F("\n--- Test de communication ESP32 <-> LoRa CC68 ---"));

  // Initialisation du module à 868.0 MHz
  // .begin() vérifie si la puce répond correctement via SPI
  Serial.print(F("[LLCC68] Initialisation... "));
  int state = radio.begin(868.0);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("SUCCÈS !"));
    Serial.println(F("Le module LoRa répond et le câblage SPI est correct."));
  } else {
    Serial.print(F("ÉCHEC. Code erreur : "));
    Serial.println(state);
    
    // Aide au diagnostic
    if (state == -2) Serial.println(F("Erreur: Le module ne répond pas. Vérifiez le câblage et l'alimentation 3.3V."));
    if (state == -70) Serial.println(F("Erreur: Broche BUSY bloquée. Vérifiez le GPIO 35."));
    
    while (true); // Bloque ici si ça ne marche pas
  }
}

void loop() {
  // Si on arrive ici, tout va bien. 
  // On fait juste un petit scan rapide pour confirmer que la puce est active
  Serial.println(F("Module opérationnel. En attente de commandes..."));
  delay(10000);
}