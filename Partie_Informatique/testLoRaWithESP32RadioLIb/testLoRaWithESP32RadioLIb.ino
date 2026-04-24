#include <RadioLib.h>
#include <esp_sleep.h>

#define LORA_NSS 5
#define LORA_DIO1 32
#define LORA_NRST 27
#define LORA_BUSY 33

LLCC68 radio = new Module(LORA_NSS, LORA_DIO1, LORA_NRST, LORA_BUSY);

unsigned long lastRxTime = 0;
const unsigned long INACTIVITY_TIMEOUT = 7000;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== ESP32 <-> LoRa LLCC68 - Wake-up   ===\n");
  int state = -1;
  for (int i = 0; i < 5; i++) {  // max 5 tentatives
    state = radio.begin(868.0, 125.0, 9, 7, 0x12, 17, 8, 1.6f);
    Serial.print("begin() attempt ");
    Serial.print(i + 1);
    Serial.print(" = ");
    Serial.println(state);
    if (state == RADIOLIB_ERR_NONE) break;
    delay(1000);
  }

  if (state != RADIOLIB_ERR_NONE) {
    Serial.println("Init FAILED after 5 attempts. Check wiring / power.");
    while (true);
  }
  Serial.println("LoRa initialized SUCCESSFULLY!");
  radio.setDio1Action(onPacketReceived);
  radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF);
  lastRxTime = millis();
  Serial.println("Listening... ESP32 will sleep after 10s of inactivity");
}

void onPacketReceived() {
  lastRxTime = millis();
}

void loop() {
  String received = "";
  // On ne lit la radio QUE si la pin DIO1 nous dit qu'un message est là
  if (digitalRead(LORA_DIO1) == HIGH) {
    int rxState = radio.readData(received);

    if (rxState == RADIOLIB_ERR_NONE) {
      Serial.println("\n=== MESSAGE RECEIVED ===");
      Serial.print("Msg : ");
      Serial.println(received);
      Serial.print("RSSI: ");
      Serial.print(radio.getRSSI());
      Serial.println(" dBm");
      Serial.print("SNR : ");
      Serial.print(radio.getSNR());
      Serial.println(" dB");

      lastRxTime = millis();  // Reset du timer d'inactivité
    }
    // TRÈS IMPORTANT : On relance la réception pour effacer le flag interne et continuer l'écoute
    radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF);
  }

  if (millis() - lastRxTime >= INACTIVITY_TIMEOUT) {
    Serial.println("\n[INFO] Inactivité -> Configuration Deep Sleep...");

    // 1. On s'assure que DIO1 est bien à 0 avant de partir
    radio.standby();
    radio.clearDio1Action();  // On enlève l'action actuelle
    // 2. On reconfigure DIO1 pour qu'il ne réagisse QU'AU MESSAGE reçu
    radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF);
    delay(100);
    Serial.flush();
    // ==== Config DEEP SLEEP Mode 
    esp_sleep_enable_ext0_wakeup((gpio_num_t)LORA_DIO1, 1); 
    Serial.println("ESP32 entering DEEP SLEEP mode... ");
    delay(100);
    esp_deep_sleep_start();
  }
}