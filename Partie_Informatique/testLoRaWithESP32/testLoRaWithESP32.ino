#include <RadioLib.h>

#define LORA_NSS   5
#define LORA_DIO1  34
#define LORA_NRST  27
#define LORA_BUSY  35

LLCC68 radio = new Module(LORA_NSS, LORA_DIO1, LORA_NRST, LORA_BUSY);

void setup() {
  Serial.begin(115200);
  delay(1000);  // Give time for serial

  Serial.println("\n=== LLCC68 Init Test (XTAL mode) ===");
  Serial.print("Pins: NSS="); Serial.print(LORA_NSS);
  Serial.print(" DIO1="); Serial.print(LORA_DIO1);
  Serial.print(" NRST="); Serial.print(LORA_NRST);
  Serial.print(" BUSY="); Serial.println(LORA_BUSY);

  pinMode(LORA_BUSY, INPUT);
  pinMode(LORA_DIO1, INPUT);

  Serial.print("Initial BUSY state: "); Serial.println(digitalRead(LORA_BUSY));

  // Force XTAL (crystal) – this is key for standard LoRa-CC68-868
  int state = radio.begin(868.0, 125.0, 9, 7, 0x12, 10, 8, 0.0f);

  Serial.print("[LLCC68] begin() returned: "); Serial.println(state);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("SUCCESS! Module ready (using crystal oscillator).");
  } else {
    Serial.print("FAILED – code "); Serial.println(state);
    while (true);
  }
}

void loop() {
  Serial.println("In the main loop...");
  delay(3000);
}