/****************************************************************************/
/*                                                                          */
/* Copyright (C) 2024-2024 Gauthier Brière (gauthier.briere "at" gmail.com) */
/*                                                                          */
/* This file: led.h is part of WeTimer / WeDT                               */
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

#ifndef led_h
  #define led_h

  #define LED_RED   D5 // D5
  #define LED_GREEN 13 // D7
  #define LED_BLUE  15 // D8

  #define LED_ALLUMEE HIGH
  #define LED_ETEINTE LOW

  #define POWER_OFF    0
  #define POWER_RED   63
  #define POWER_GREEN 16
  #define POWER_BLUE  32
  
  #define ROUGE  0xff0000
  #define JAUNE  0xffff00
  #define VERT   0x00ff00
  #define CIAN   0x00ffff
  #define BLEU   0x0000ff
  #define MAUVE  0xff00ff

  extern u_int32_t couleurActive;

  void allumeLED(u_int32_t couleur);
  void eteindLED(void);
  u_int32_t getCouleurActive(void);
  String getColorName(u_int32_t couleur);

#endif
