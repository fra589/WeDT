/****************************************************************************/
/*                                                                          */
/* Copyright (C) 2024-2024 Gauthier Brière (gauthier.briere "at" gmail.com) */
/*                                                                          */
/* This file: wifi.h is part of WeTimer / WeDT                              */
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

#ifndef wifi_h
  #define wifi_h

  // Paramètres WiFi par défaut
  #define DEFAULT_AP_SSID    "WeDT_" // SSID du boitier DT
  #define DEFAULT_AP_PWD     ""      // WPA-PSK/WPA2-PSK AP
  #define DEFAULT_CLI_SSID   ""      // SSID client (le RDT se connecte si défini)
  #define DEFAULT_CLI_PWD    ""      // WPA-PSK/WPA2-PSK client
  #define DEFAULT_AP_CHANNEL 3

  #ifdef DEBUG
    void onStationConnected(const WiFiEventSoftAPModeStationConnected& evt);
    void onStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt);
  #endif

  void wifiApInit(void);
  void wifiClientInit(void);
  String getWifiNetworks(void);
  bool isIp(String str);
  String IPtoString(IPAddress ip);
  String macToString(const unsigned char* mac);

#endif
