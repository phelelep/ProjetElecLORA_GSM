#include <SPI.h>
#include <LoRa.h>

#define LORA_NSS   5
#define LORA_DIO1  34    // = irqPin pour LoRa.h
#define LORA_NRST  27
// BUSY = 35 → pas utilisé par LoRa.h

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Test LoRa.h - SX127x seulement !");

  LoRa.setPins(LORA_NSS, LORA_NRST, LORA_DIO1);

  if (!LoRa.begin(868E6)) {           // 868 MHz
    Serial.println("Echec LoRa.begin()");
    while (true);
  }

  Serial.println("LoRa OK !");
  LoRa.setSyncWord(0x12);             // même sync word que RadioLib
}

void loop() {
  Serial.println("Module SX127x prêt");
  delay(5000);
}