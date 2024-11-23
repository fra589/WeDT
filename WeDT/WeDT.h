/****************************************************************************/
/*                                                                          */
/* Copyright (C) 2024-2024 Gauthier Brière (gauthier.briere "at" gmail.com) */
/*                                                                          */
/* This file: WeDT.h is part of WeTimer / WeDT                              */
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

#ifndef WeDT_h
  #define WeDT_h

  // Pour DEBUG sur port série
  #define DEBUG
  #define DEBUG_WEB
  //#define DEBUG_WIFI // Commenter également Serial.setDebugOutput(true) dans WeDT.ino

  #include <string.h>
  #include <EEPROM.h>
  #include <LittleFS.h>
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
  #include <DNSServer.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>

  #include "webserver.h"
  #include "webclient.h"
  #include "wifi.h"
  #include "led.h"
  #include "eeprom.h"

  // Organisatopn / produit
  #define ORG_NAME           "fra589"
  #define COPYRIGHT          "G.Brière 2024-2024"
  #define APP_NAME           "WeDT"
  #define APP_VERSION_MAJOR  "0"
  #define APP_VERSION_MINOR  "1"
  #define APP_VERSION_DATE   "20241123"
  #define APP_VERSION_STRING "v" APP_VERSION_MAJOR "." APP_VERSION_MINOR "." APP_VERSION_DATE
  #define APP_NAME_VERSION   APP_NAME " - " APP_VERSION_STRING "." APP_VERSION_DATE "\0"


  #define PIN_SWITCH       0 // D3

  //----------------------------------------------------------------------------------------------------
  // Valeurs d'état du switch
  //----------------------------------------------------------------------------------------------------
  #define SWITCH_ON  0
  #define SWITCH_OFF 1

  //----------------------------------------------------------------------------------------------------
  // Variables globales
  //----------------------------------------------------------------------------------------------------
  #define MAX_SSID_LEN 32 // Longueur maxi d'un SSID
  #define MAX_PWD_LEN  63 // Longueur maxi des mots de passe WiFi
  extern char cli_ssid[MAX_SSID_LEN];
  extern char cli_pwd[MAX_PWD_LEN];
  extern char ap_ssid[MAX_SSID_LEN];
  extern char ap_pwd[MAX_PWD_LEN];
  // hostname for mDNS. Should work at least on windows. Try http://WeTimer.local
  extern const char *myHostname;
  // DNS server
  #define DNS_PORT 53
  extern DNSServer dnsServer;
  // Web server
  extern ESP8266WebServer server;
  /* Soft AP network parameters */
  extern IPAddress apIP;
  extern IPAddress netMsk;

#endif
