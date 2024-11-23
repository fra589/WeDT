/****************************************************************************/
/*                                                                          */
/* Copyright (C) 2024-2024 Gauthier Brière (gauthier.briere "at" gmail.com) */
/*                                                                          */
/* This file: led.cpp is part of WeTimer / WeDT                             */
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

u_int32_t couleurActive = 0x00000000;

void allumeLED(u_int32_t couleur) {

  #ifdef DEBUG
    Serial.printf("Allumage LED %s : ", getColorName(couleur).c_str());
  #endif
  
  if (couleur & ROUGE) {
    #ifdef DEBUG
      Serial.printf("rouge ");
      Serial.flush();
    #endif
    analogWrite(LED_RED, POWER_RED);
  } else {
    analogWrite(LED_RED, POWER_OFF);
  }
  if (couleur & VERT) {
    #ifdef DEBUG
      Serial.printf("verte ");
      Serial.flush();
    #endif
    analogWrite(LED_GREEN, POWER_GREEN);
  } else {
    analogWrite(LED_GREEN, POWER_OFF);
  }
  if (couleur & BLEU) {
    #ifdef DEBUG
      Serial.printf("bleu ");
      Serial.flush();
    #endif
    analogWrite(LED_BLUE, POWER_BLUE);
  } else {
    analogWrite(LED_BLUE, POWER_OFF);
  }
  #ifdef DEBUG
    Serial.printf("\n");
    Serial.flush();
  #endif

  couleurActive = couleur;

}

void eteindLED(void) {

  #ifdef DEBUG
    Serial.printf("Extintion LEDs : ");
  #endif

  #ifdef DEBUG
    Serial.println("rouge ");
    Serial.flush();
  #endif
  digitalWrite(LED_RED, POWER_OFF);
  #ifdef DEBUG
    Serial.println("verte ");
    Serial.flush();
  #endif
  digitalWrite(LED_GREEN, POWER_OFF);
  #ifdef DEBUG
    Serial.println("bleu\n");
    Serial.flush();
  #endif
  digitalWrite(LED_BLUE, POWER_OFF);

  couleurActive = 0x00000000;

}

u_int32_t getCouleurActive(void) {

  return couleurActive;

}

String getColorName(u_int32_t couleur) {
  
  switch (couleur) {
    case ROUGE:
      return "Rouge";
      break;
    case JAUNE:
      return "Jaune";
      break;
    case VERT:
      return "Vert";
      break;
    case CIAN:
      return "Cian";
      break;
    case BLEU:
      return "Bleu";
      break;
    case MAUVE:
      return "Mauve";
      break;
    default:
      return "";
  }

}
