#include <RadioLib.h>

#define LORA_NSS   5
#define LORA_DIO1  34
#define LORA_NRST  27
#define LORA_BUSY  35

LLCC68 radio = new Module(LORA_NSS, LORA_DIO1, LORA_NRST, LORA_BUSY);

void setup() {
  Serial.begin(115200);
  delay(400);

  Serial.print(F("[LLCC68] Initialising... "));

  pinMode(LORA_BUSY, INPUT);
  pinMode(LORA_DIO1, INPUT);

  int state = radio.begin(868.0, 125.0, 9, 7, 0x12, 10, 8);  // 0.0 = XTAL

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("SUCCESS!"));
  } else {
    Serial.print(F("FAILED, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  Serial.println(F("Ready!"));
  delay(5000);
}