/****************************************************************************/
/*                                                                          */
/* Copyright (C) 2024-2024 Gauthier Brière (gauthier.briere "at" gmail.com) */
/*                                                                          */
/* This file: WeDT.ino is part of WeTimer / WeDT                            */
/*                                                                          */
/* WeTimer / WeDT is free software: you can redistribute it and/or modify   */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* WeTimer / WeDT is distributed in the hope that it will be useful, but    */
/* WITHOUT ANY WARRANTY; without even the implied warranty of               */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*                                                                          */
/****************************************************************************/

#include "WeDT.h"

//--------------------------------------------------------------------
// Variables globales
//--------------------------------------------------------------------

// WiFi
char cli_ssid[MAX_SSID_LEN] = DEFAULT_CLI_SSID;
char cli_pwd[MAX_PWD_LEN]   = DEFAULT_CLI_PWD;
char ap_ssid[MAX_SSID_LEN]  = DEFAULT_AP_SSID;
char ap_pwd[MAX_PWD_LEN]    = DEFAULT_AP_PWD;
// hostname for mDNS (http://WeDT.local)
const char *myHostname = APP_NAME;
// DNS server
DNSServer dnsServer;
// Web server
ESP8266WebServer server(80);
/* Soft AP network parameters */
IPAddress apIP(10, 11, 11, 11);
IPAddress netMsk(255, 255, 255, 0);
// Switch status
int switchStatus = 0;
uint32_t oldCouleur = 0;


//--------------------------------------------------------------------
// Initialisation
//--------------------------------------------------------------------
void setup() {

  // Try pushing frequency to 160MHz.
  bool update_cpu_freq = system_update_cpu_freq(160);
  // Set CPU frequency to 80MHz.
  //bool update_cpu_freq = system_update_cpu_freq(80);

  #ifdef DEBUG
    Serial.begin(115200);
    delay(100);
    while (!Serial) {
      delay(1);
      yield();
    }
    Serial.flush();
    Serial.printf("\n");
    Serial.flush();
    int cpuFreq = system_get_cpu_freq();
    Serial.printf("Starting %s on ESP8266@%dMHz (system_update_cpu_freq() = %s)...\n\n", APP_NAME_VERSION, cpuFreq, update_cpu_freq?"true":"false");
    Serial.flush();
    #ifdef DEBUG_WIFI
      //Serial.setDebugOutput(true);
    #endif
  #endif

  // Allumage de la LED builtin pendant l'initialisation
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //pinMode(LED_GND, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  // Allumage LED ROUGE
  allumeLED(ROUGE);

  pinMode(PIN_SWITCH, INPUT_PULLUP);

  // Récupération des paramètres EEPROM
  getEepromStartupData();


  // Initialisation AP WiFi
  wifiApInit();
  // Démarrage du serveur web
  webServerInit();
  // Connection à la minuterie
  wifiClientInit();

  // Regarde le status du bouton
  switchStatus = digitalRead(PIN_SWITCH);
  // Memorise la couleur active
  oldCouleur = getCouleurActive();

  // Eteind la LED builtin
  digitalWrite(LED_BUILTIN, HIGH);

}

void loop() {

  int new_status = digitalRead(PIN_SWITCH);

  if (new_status != switchStatus) {
    #ifdef DEBUG
      Serial.printf("Statue du bouton = %s\n", new_status?"relaché":"appuyé");
    #endif
    if (new_status == SWITCH_ON) {
      // Memorise la couleur active
      oldCouleur = getCouleurActive();
      #ifdef DEBUG
        Serial.printf("Old couleur active = 0x%08x\n", couleurActive);
      #endif
      // Allume la LED en bleu
      allumeLED(BLEU);
      // Envoi la requette RDT
      sendRDT();
    } else {
      // restaure la couleur précédente de la LED
      allumeLED(oldCouleur);
    }
    switchStatus = new_status;
  }

  if (WiFi.status() == WL_CONNECTED) {
    if ((switchStatus != SWITCH_ON) && (getCouleurActive() != VERT)) {
      // Connecté : LED verte
      allumeLED(VERT);
    }
  } else {
    if ((switchStatus != SWITCH_ON) && (getCouleurActive() != ROUGE)) {
      // Non connecté : LED rouge
      allumeLED(ROUGE);
    }
  }

  // Process DNS
  dnsServer.processNextRequest();
  yield();
  delay(5);
  // Mise à jour MDNS
  MDNS.update();
  yield();
  delay(5);
  // Process HTTP
  server.handleClient();
  yield();
  delay(5);

}
