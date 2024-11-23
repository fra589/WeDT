/****************************************************************************/
/*                                                                          */
/* Copyright (C) 2024-2024 Gauthier Brière (gauthier.briere "at" gmail.com) */
/*                                                                          */
/* This file: webserver.h is part of WeTimer / WeDT                         */
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

#ifndef webserver_h
  #define webserver_h
  
  //#define GENERATE_204

  #define ROOT_FILE "/index.html"

  void webServerInit(void);
  bool captivePortal(void);
  void handleRoot(void);
  #ifdef GENERATE_204
    void generate_204(void);
  #endif
  void handleNotFound(void);
  bool handleFileRead(String path);
  void handleGetNetworks(void);
  void handleDeconnection(void);
  void handleWifiConnect(void);

#endif
