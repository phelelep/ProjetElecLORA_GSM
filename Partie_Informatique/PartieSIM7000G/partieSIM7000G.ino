#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Définir le tampon RX à 1Ko
#define SerialAT Serial1

//=========Partie traitement donnes ruches==============

// 1. Structure pour représenter les données d'une ruche
struct rucheData {
  int id;
  float temperature;
  float poids;
};
// declaration de nombre total de ruche
const int  nrRuches = 10;
rucheData ruches[nrRuches]; 

//simuler les donnes 
void simulData(){
  for (int i =0; i<nrRuches; i++){
      ruches[i].id = i+1;
      ruches[i].temperature = random(0, 400) / 10.0; // du 0 à 40;
      ruches[i].poids= random(100000, 400000) / 10.0; // du 10kg à 30 kg
  }
}

String serializeData() {
  String payload = "[";
  for (int i = 0; i < nrRuches; i++) {
    payload += "{";
    payload += "\"id\":" + String(ruches[i].id) + ",";
    payload += "\"temp\":" + String(ruches[i].temperature) + ",";
    payload += "\"weight\":" + String(ruches[i].poids);
    payload += "}";
    if (i < nrRuches - 1) {
      payload += ",";
    }
  }
  payload += "]";
  return payload;
}
//======================================================

// Voir toutes les commandes AT, si souhaité
//#define DUMP_AT_COMMANDS

// Définir le code PIN de la carte SIM, si nécessaire
#define GSM_PIN ""

// Vos identifiants GPRS, si vous en avez
const char apn[]  = "sl2sfr";     //DÉFINISSEZ VOTRE APN
const char gprsUser[] = "";
const char gprsPass[] = "";

// Définir un numéro de téléphone pour tester les SMS (doit être au format international avec le "+" devant)
#define SMS_TARGET  "+33634165337" // Nr du Daniel

// --- AJOUTS POUR HTTP ---
const char server[] = "rucher.polytech.unice.fr";
const int port = 80; // Port HTTP standard


#include <TinyGsmClient.h>
//#include <Ticker.h>

#ifdef DUMP_AT_COMMANDS  // si activé, nécessite la lib streamDebugger
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, Serial);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

// --- AJOUT CLIENT HTTP ---
TinyGsmClient client(modem);
// -------------------------


#define uS_TO_S_FACTOR 1000000ULL  // Facteur de conversion de microsecondes en secondes
#define TIME_TO_SLEEP  60          // Durée pendant laquelle l'ESP32 dormira (en secondes)

#define UART_BAUD   115200
//#define PIN_DTR     25
#define PIN_RX      16
#define PIN_TX      17
#define PWR_KEY     4
#define LED_PIN     12


//======================================
//    FONCTION HTTP POST
//======================================
void sendDataOverHTTP() {
  Serial.println("--- DÉBUT ENVOI HTTP POST ---");
  simulData(); 
  String payload = serializeData(); // create a JSON
  
  Serial.println("Payload JSON créé :");
  Serial.println(payload);

  Serial.print("Connexion à ");
  Serial.println(server);
  if (client.connect(server, port)) {
      Serial.println("Connecté. Envoi de la requête POST...");

      // Construire et envoyer la requête HTTP POST
      // Assurez-vous que le chemin (le "/" après POST) est correct.
      // Si l'API attend les données sur http://.../api/data, mettez "/api/data"
      client.print(String("POST / HTTP/1.1\r\n")); 
      client.print(String("Host: ") + server + "\r\n");
      client.print("Connection: close\r\n");
      client.print("Content-Type: application/json\r\n");
      client.print(String("Content-Length: ") + payload.length() + "\r\n");
      client.print("\r\n"); // Ligne vide OBLIGATOIRE (fin des en-têtes)
      client.print(payload);   // Envoyer le corps de la requête (votre JSON)

      Serial.println("Requête envoyée.");

      // 4. Attendre et lire la réponse du serveur
      long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) { // 5 secondes de timeout
          Serial.println(">>> Délai d'attente réponse serveur !");
          client.stop();
          break;
        }
      }
  
      Serial.println("Réponse du serveur :");
      while (client.available()) {
        Serial.write(client.read());
      }

      // 5. Fermer la connexion
      client.stop();
      Serial.println("\nConnexion HTTP fermée.");

  } else {
      Serial.println("Échec de la connexion au serveur HTTP.");
  }

  Serial.println("--- FIN ENVOI HTTP POST ---");
}


// ==================== SETUUUUUUP =======================
void setup(){
  // Définir le débit en bauds de la console
  Serial.begin(115200);
  delay(10);
  
  // Éteindre la LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.println("\nPatientez... Démarrage du modem.");

  // Allumage du modem via PWR_KEY
  pinMode(PWR_KEY, OUTPUT);
  digitalWrite(PWR_KEY, HIGH);
  delay(300);
  digitalWrite(PWR_KEY, LOW);
  
  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

  // Redémarrer prend un certain temps
  Serial.println("Initialisation du modem...");
  if (!modem.init()) {
    Serial.println("Échec d'initialisation du modem, tentative de continuation sans redémarrage");
  }

  String modemInfo = modem.getModemInfo();
  Serial.println("Infos modem : " + modemInfo);


  // Déverrouillez votre carte SIM avec un PIN si nécessaire
  if ( GSM_PIN && modem.getSimStatus() != 3 ) {
      modem.simUnlock(GSM_PIN);
  }

  // --- CONFIGURATION RÉSEAU (DÉPLACÉE DEPUIS LOOP) ---
  Serial.println("Configuration du mode réseau...");
  
  /*
    2 Automatique
    13 GSM seulement
    38 LTE seulement
    51 GSM et LTE uniquement
  */
  String res;
  res = modem.setNetworkMode(2); // mode automatique
  if (res != "1") {
    DBG("setNetworkMode échoué ");
  }
  delay(200);

  /*
    1 CAT-M
    2 NB-IoT
    3 CAT-M et NB-IoT
  */
  res = modem.setPreferredMode(1); // mode CAT-M
  if (res != "1") {
    DBG("setPreferredMode échoué ");
  }
  delay(200);

  // S'assurer que le modem est en pleine fonctionnalité
  modem.sendAT("+CFUN=1 ");
  if (modem.waitResponse(10000L) != 1) {
    DBG(" +CFUN=1 échoué ");
  }
  
  Serial.println("Configuration modem terminée.");
  // --- FIN CONFIGURATION RÉSEAU ---
}


//====================== LOOOOOOOOOOOP ===========================

void loop(){
  Serial.println("\n\n--- DÉBUT DU CYCLE LOOP ---");

  Serial.println("Attente du réseau...");
  if (!modem.waitForNetwork()) 
  {
    Serial.println("Échec réseau, passage au deep-sleep.");
  }
  else
  {
    Serial.println("Réseau trouvé. Connexion GPRS...");
    
    // Connexion GPRS
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) 
    {
      Serial.println("Échec de connexion GPRS.");
    } 
    else 
    {
      Serial.println("GPRS Connecté.");

      // -------- 1. ENVOI DES DONNÉES HTTP --------
      sendDataOverHTTP();
      
      // -------- 2. TEST ENVOI SMS --------
      // (Nous avons récupéré l'IMEI au cas où, mais il est préférable de le faire une fois dans setup)
      String imei = modem.getIMEI();
      String res;
      res = modem.sendSMS(SMS_TARGET, String("Project LoRa GSM, sender IMEI: ") + imei);
      DBG("Statut SMS:", res ? "Envoyé" : "Échec");

      // -------- 3. TESTS GPRS (Optionnel mais utile) --------
      Serial.println("\n--- INFOS DIAGNOSTIC GPRS ---");
      String ccid = modem.getSimCCID();
      Serial.println("CCID : " + ccid);
      Serial.println("IMEI : " + imei); // Réutilisation
      String cop = modem.getOperator();
      Serial.println("Opérateur : " + cop);
      IPAddress local = modem.localIP();
      Serial.println("IP locale : " + String(local));
      int csq = modem.getSignalQuality();
      Serial.println("Qualité du signal : " + String(csq));
      Serial.println("--- FIN INFOS GPRS ---");

      // -------- 4. DÉCONNEXION GPRS --------
      modem.gprsDisconnect();
      if (!modem.isGprsConnected()) {
        Serial.println("GPRS déconnecté");
      } else {
        Serial.println("Déconnexion GPRS : Échec");
      }
    }
  }

  // -------- 5. EXTINCTION ET VEILLE --------
  Serial.println("Extinction du modem...");
  modem.poweroff();
  
  Serial.println("Mise en veille profonde de l'ESP32 pour 60 secondes...");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  delay(200); // Laisse le temps aux messages série de s'imprimer
  esp_deep_sleep_start();

}