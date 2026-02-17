#include <RadioLib.h>
// NSS: 5, DIO1: 33, NRST: 27, BUSY: 35
LLCC68 radio = new Module(5, 34, 14, 35);

void setup() {
  Serial.begin(115200);
  delay(2000); // Laisse le temps au port série de s'ouvrir

  Serial.println(F("\n--- Test de communication ESP32 <-> LoRa CC68 ---"));
  Serial.print(F("[LLCC68] Initialisation... "));

 int state = radio.begin(868.0, 125.0, 9, 7, 0x12, 10, 8, 1.6);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("SUCCÈS !"));
    Serial.println(F("Le module LoRa répond et le câblage SPI est correct."));
  } else {
    Serial.print(F("ÉCHEC. Code erreur : "));
    Serial.println(state);

    while (true); // Bloque ici si ça ne marche pas
  }
}

void loop() {
  Serial.println(F("Module opérationnel. En attente de commandes..."));
  delay(10000);
}