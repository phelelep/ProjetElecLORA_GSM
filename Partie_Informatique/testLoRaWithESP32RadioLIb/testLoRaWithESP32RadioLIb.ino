#include <RadioLib.h>
#include <esp_sleep.h>

#define LORA_NSS   5
#define LORA_DIO1  36 // ce pin réveille le module ESP32 
#define LORA_NRST  27
#define LORA_BUSY  33

LLCC68 radio = new Module(LORA_NSS, LORA_DIO1, LORA_NRST, LORA_BUSY);

const unsigned long INACTIVITY_TIMEOUT = 10000; 

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== LoRa LLCC68 - Wake-up + Deep Sleep  ===\n");

  int state = -1;
  for (int i = 0; i < 5; i++) {        // max 5 tentatives
    state = radio.begin(868.0, 125.0, 9, 7, 0x12, 17, 8, 1.6f);
    Serial.print("begin() attempt "); Serial.print(i+1); 
    Serial.print(" = "); Serial.println(state);
    if (state == RADIOLIB_ERR_NONE) break;
    delay(1000);
  }

  if (state != RADIOLIB_ERR_NONE) {
    Serial.println("Init FAILED after 5 attempts. Check wiring / power.");
    while (true);
  }

  Serial.println("LoRa initialized SUCCESSFULLY!");
  radio.startReceive();
  Serial.println("Listening... ESP32 will sleep after 10s of inactivity");
}


void loop() {

  String received = "";
  int rxState = radio.receive(received);

  if (rxState == RADIOLIB_ERR_NONE) {
    Serial.println("\n=== MESSAGE RECEIVED ===");
    Serial.print("Msg  : "); Serial.println(received);
    Serial.print("RSSI : "); Serial.print(radio.getRSSI()); Serial.println(" dBm");
    Serial.print("SNR  : "); Serial.print(radio.getSNR()); Serial.println(" dB");
    Serial.println("-------------------");

    radio.startReceive();        // Re-arm for next packet
    delay(1000);
  }

  // Gestion du Deep Sleep
  static unsigned long lastRx = millis();
  if (millis() - lastRx >= INACTIVITY_TIMEOUT) {
    Serial.println("\n[INFO] 10 seconds inactivity → Entering Deep Sleep...");
    delay(100);
    Serial.flush();                     // Attendre que le message soit envoyé
    
    
    // Configurer le wake-up sur DIO1 (niveau HIGH)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)LORA_DIO1, HIGH);
    esp_deep_sleep_start();             // L'ESP32 s'endort ici
  }

  delay(50);
}