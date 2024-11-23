/****************************************************************************/
/*                                                                          */
/* Copyright (C) 2024-2024 Gauthier Brière (gauthier.briere "at" gmail.com) */
/*                                                                          */
/* This file: wifi.cpp is part of WeTimer / WeDT                            */
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



#ifdef DEBUG
  // Prise en charge des évennements
  WiFiEventHandler stationConnectedHandler;
  WiFiEventHandler stationDisconnectedHandler;
  WiFiEventHandler probeRequestPrintHandler;
  
  void onStationConnected(const WiFiEventSoftAPModeStationConnected& evt) {
    Serial.print("Station connected: ");
    Serial.print(macToString(evt.mac));
    Serial.print(" aid = ");
    Serial.println(evt.aid);
  }

  void onStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt) {
    Serial.print("Station disconnected: ");
    Serial.println(macToString(evt.mac));
  }
#endif

// Configuration WiFi AP 
void wifiApInit(void) {

  // Soft AP network parameters
  IPAddress apIP(10, 0, 0, 11);
  IPAddress gateway(10, 0, 0, 11);
  IPAddress netMsk(255, 255, 255, 0);
  int ssid_hidden = 0; // 1 pour masquer le SSID
  int max_connection = 4; // 8 au maximum possible, 4 max préconisé pour les perfos et la stabilité

  #ifdef DEBUG
    Serial.printf("\nConfiguring access point, SSID = <%s>...\n", ap_ssid);
    Serial.flush();
  #endif

  WiFi.softAPConfig(apIP, gateway, netMsk);
  // AP ouverte si mot de passe vide ou null
  WiFi.softAP(ap_ssid, ap_pwd, DEFAULT_AP_CHANNEL, ssid_hidden, max_connection);
  delay(500);

  #ifdef DEBUG
    Serial.printf("AP IP address  = %s\n", WiFi.softAPIP().toString().c_str());
    Serial.printf("AP MAC address = %s\n", WiFi.softAPmacAddress().c_str());
    Serial.printf("Wifi channel   = %d\n", WiFi.channel());
    Serial.flush();
    // Register event handlers.
    // Callback functions will be called as long as these handler objects exist.
    // Call "onStationConnected" each time a station connects
    stationConnectedHandler = WiFi.onSoftAPModeStationConnected(&onStationConnected);
    // Call "onStationDisconnected" each time a station disconnects
    stationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected(&onStationDisconnected);
  #endif

  // Setup the DNS server redirecting all domains to the apIP
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  //Start mDNS with APP_NAME
  if (MDNS.begin(myHostname, WiFi.localIP())) {
    ;
    #ifdef DEBUG
      Serial.println("MDNS started");
    #endif
  } else {
    ;
    #ifdef DEBUG
      Serial.println("MDNS failed");
    #endif
  }

}

void wifiClientInit(void) {

  unsigned long debut;

  // Empeche le wifi client de se connecter avec d'anciens paramètres résiduels en EEPROM.
  WiFi.setAutoReconnect (false );

  // Connection Cliente si SSID défini
  if (cli_ssid[0] != '\0') {
    #ifdef DEBUG
      Serial.printf("Connexion à %s...\n", cli_ssid);
      Serial.flush();
    #endif
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    // Allumage LED jaune
    allumeLED(JAUNE);
    debut = millis();
    WiFi.begin(cli_ssid, cli_pwd);
    while (WiFi.status() != WL_CONNECTED) {
      delay(250);
      if (millis() - debut > 15000) {
        break; // Timeout = 15 secondes
      }
    }

    #ifdef DEBUG
      if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("OK\n");
        Serial.printf("Local  IP = %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("Remote IP = %s\n", WiFi.gatewayIP().toString().c_str());
        // Allumage LED verte
        allumeLED(VERT);
      } else {
        Serial.printf("FAIL\n");
        // Allumage LED ROUGE
        allumeLED(ROUGE);
      }
      Serial.flush();
    #endif
  }

}

// renvoie en format XML la liste des SSID scannés 
String getWifiNetworks() {
  // Voir https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/scan-examples.html#scan
  // https://randomnerdtutorials.com/esp8266-nodemcu-wi-fi-manager-asyncwebserver/

  String XML = "";
  int nReseaux;
  int i;

  #ifdef DEBUG_WEB
    Serial.printf("Entrée dans getWifiNetworks()\n");
  #endif

  if(WiFi.isConnected()) {
    #ifdef DEBUG_WEB
      Serial.printf("WiFi.isConnected() = true\n");
    #endif
    XML += "  <activeNetwork>\n";
    XML += "    <SSID>";
    XML += WiFi.SSID();
    XML += "</SSID>\n";
    XML += "    <PSK>";
    XML += WiFi.psk();
    XML += "</PSK>\n";
    XML += "    <RSSI>";
    XML += WiFi.RSSI();
    XML += "</RSSI>\n";
    XML += "    <channel>";
    XML += WiFi.channel();
    XML += "</channel>\n";
    XML += "    <localip>";
    XML += IPtoString(WiFi.localIP());
    XML += "</localip>\n";
    XML += "  </activeNetwork>\n";
  } else if (cli_ssid[0] != '\0') {
    // Non connecté mais SSID définit
    #ifdef DEBUG_WEB
      Serial.printf("WiFi.isConnected() = false, cli_ssid = %s\n", cli_ssid);
    #endif
    XML += "  <activeNetwork>\n";
    XML += "    <SSID>";
    XML += String(cli_ssid);
    XML += "</SSID>\n";
    XML += "    <PSK>";
    XML += String(cli_pwd);
    XML += "</PSK>\n";
    XML += "    <RSSI/>\n";
    XML += "    <channel/>\n";
    XML += "    <localip>*NOT CONNECTED*</localip>\n";
    XML += "  </activeNetwork>\n";
  }
  nReseaux = WiFi.scanNetworks();
  for (i = 0; i < nReseaux; i++) {
    XML += "  <network>\n";
    XML += "    <SSID>";
    XML += WiFi.SSID(i);
    XML += "</SSID>\n";
    XML += "    <channel>";
    XML += WiFi.channel(i);
    XML += "</channel>\n";
    XML += "    <RSSI>";
    XML += WiFi.RSSI(i);
    XML += "</RSSI>\n";
    XML += "    <encryption>";
    switch (WiFi.encryptionType(i)) {
      case (ENC_TYPE_NONE):
        XML += "none";
        break;
      case (ENC_TYPE_WEP):
        XML += "WEP";
        break;
      case (ENC_TYPE_TKIP):
        XML += "WPA-PSK";
        break;
      case (ENC_TYPE_CCMP):
        XML += "WPA2-PSK";
        break;
      case (ENC_TYPE_AUTO):
        XML += "Auto";
        break;
      default:
        XML += "Inconnue";
    }
    XML += "</encryption>\n";
    XML += "  </network>\n";
  }
  WiFi.scanDelete();
  #ifdef DEBUG_WEB
    Serial.printf("getWifiNetworks(): Fin du scan des réseaux WiFi.\n", cli_ssid);
  #endif

  return XML;

}

bool isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

String IPtoString(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String((ip >> (8 * 3)) & 0xFF);
  return res;
}

String macToString(const unsigned char* mac) {
  char buf[18];
  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

