#include <SPI.h>
#include <LoRa.h>

unsigned long lastTx = 0;
const unsigned long TX_INTERVAL = 10000;   // envoi toutes les x secondes

int txCount = 0;
int rxCount = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("\n=== Heltec WiFi LoRa 32 V2.1 - Bidirectionnel ===");

  // Configuration des pins pour Heltec V2.1
  LoRa.setPins(18, 14, 26);     // NSS, RST, DIO0 (IRQ)

  if (!LoRa.begin(868E6)) {     // 868 MHz
    Serial.println("LoRa init FAILED !");
    while (true);
  }

  // Paramètres recommandés (mêmes que tu utilisais avant)
  LoRa.setSpreadingFactor(9);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(7);
  LoRa.setSyncWord(0x12);
  LoRa.enableCrc();

  Serial.println("LoRa initialized successfully !");
  Serial.println("Envoi toutes les xs + écoute permanente\n");
}

void loop() {
  // ====================== ENVOI ======================
  if (millis() - lastTx >= TX_INTERVAL) {
    lastTx = millis();
    txCount++;

    String msg = "Hello from Heltec #" + String(txCount) + " - " + String(millis() / 1000) + "s";

    Serial.print("TX #"); Serial.print(txCount); 
    Serial.print(" → "); Serial.println(msg);

    // Envoi du message
    LoRa.beginPacket();
    LoRa.print(msg);
    LoRa.endPacket();

    Serial.println("→ Envoi OK");
  }

  // ====================== RÉCEPTION ======================
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    rxCount++;
    String received = "";

    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    Serial.println("\n=== RX #" + String(rxCount) + " ===");
    Serial.print("Message reçu : "); Serial.println(received);
    Serial.print("RSSI : "); Serial.print(LoRa.packetRssi()); Serial.println(" dBm");
    Serial.print("SNR  : "); Serial.print(LoRa.packetSnr()); Serial.println(" dB");
    Serial.println("-------------------");
  }

  delay(10);   // petite pause pour ne pas surcharger le CPU
}