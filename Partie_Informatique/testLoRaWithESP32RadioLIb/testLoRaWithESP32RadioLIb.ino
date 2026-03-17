#include <RadioLib.h>

#define LORA_NSS   5
#define LORA_DIO1  36
#define LORA_NRST  27
#define LORA_BUSY  33

LLCC68 radio = new Module(LORA_NSS, LORA_DIO1, LORA_NRST, LORA_BUSY);


unsigned long lastTx = 0;
const unsigned long TX_INTERVAL = 4000; // toutes les 4 secondes

int txCount = 0;
int rxCount = 0;


void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(LORA_NRST, OUTPUT);
  pinMode(LORA_DIO1, INPUT);
  pinMode(LORA_BUSY, INPUT);

  Serial.println("\n=== Multiple LLCC68 begin() tests  ===\n");

  // Reset once at beginning
  digitalWrite(LORA_NRST, LOW);
  delay(50);
  digitalWrite(LORA_NRST, HIGH);
  delay(500);
  Serial.print("BUSY after initial reset: "); Serial.println(digitalRead(LORA_BUSY));

  int state= -1; 
  // tester jussq'au succes
  while (state != RADIOLIB_ERR_NONE)
  {
    state = radio.begin(868.0, 125.0, 9, 7, 0x12, 10, 8, 1.6f);
    Serial.print("begin() = "); Serial.println(state);
    if (state == RADIOLIB_ERR_NONE) 
    {
      Serial.print("Initialization SUCCESS! - code : "); Serial.println(state);
    }
    else 
       Serial.print("Init FAILED - code "); Serial.println(state);


    delay(1500);
  }

  // Start continuous receive mode
  radio.startReceive();
  Serial.println("Listening for packets...");
}

void loop() {
  // Envoi périodique
  if (millis() - lastTx >= TX_INTERVAL) {
    lastTx = millis();
    txCount++;

    String msg = "Test #" + String(txCount) + " - " + String(millis() / 1000) + "s";

    Serial.print("TX → "); Serial.println(msg);

    int txState = radio.transmit(msg);
    if (txState == RADIOLIB_ERR_NONE) {
      Serial.println("→ Envoi OK");
    } else {
      Serial.print("→ Échec envoi, code "); Serial.println(txState);
    }
  }

  // Réception
  String received = "";
  int rxState = radio.receive(received);

  if (rxState == RADIOLIB_ERR_NONE) {
    rxCount++;
    Serial.println("\nRX #" + String(rxCount) + " :");
    Serial.print("  Message reçu : "); Serial.println(received);
    Serial.print("  RSSI : "); Serial.print(radio.getRSSI()); Serial.println(" dBm");
    Serial.print("  SNR  : "); Serial.print(radio.getSNR()); Serial.println(" dB");
    Serial.println("-------------------");
  } else if (rxState != RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.print("Erreur réception, code "); Serial.println(rxState);
  }

  // Small delay to avoid CPU hogging
  delay(10);
}