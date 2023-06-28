/*
 *  lcd-menu.h
 *
 *  lcd menu h
 *  Created on: 2023. 4. 3
 */

#include "Arduino.h"

const char MENU_LINE_0[] PROGMEM = "                    ";
const char MENU_LINE_1[] PROGMEM = "     TANK KOSONG    ";
const char MENU_LINE_2[] PROGMEM = "      ISI TANK      ";
const char MENU_LINE_3[] PROGMEM = "                    ";

const char MENU_LINE_4[] PROGMEM = "                    ";
const char MENU_LINE_5[] PROGMEM = "  TANK BELUM PENUH  ";
const char MENU_LINE_6[] PROGMEM = "      ISI TANK      ";
const char MENU_LINE_7[] PROGMEM = "                    ";

const char MENU_LINE_8[] PROGMEM = "    SISTEM READY    ";
const char MENU_LINE_9[] PROGMEM = "   MASUKAN WAKTU    ";
const char MENU_LINE_10[] PROGMEM = "  TIME:             ";
const char MENU_LINE_11[] PROGMEM = "   * CLEAR | # SET  ";

const char MENU_LINE_12[] PROGMEM = "                    ";
const char MENU_LINE_13[] PROGMEM = "    SISTEM START    ";
const char MENU_LINE_14[] PROGMEM = "      WAIT ...      ";
const char MENU_LINE_15[] PROGMEM = "                    ";

const char MENU_LINE_16[] PROGMEM = "  Elapsed 00:00:00  ";
const char MENU_LINE_17[] PROGMEM = "  SV:               ";
const char MENU_LINE_18[] PROGMEM = "  PV:               ";
const char MENU_LINE_19[] PROGMEM = "  C for Interrupt   ";

const char MENU_LINE_20[] PROGMEM = "                   ";
const char MENU_LINE_21[] PROGMEM = "  SYSTEMS  REACHED ";
const char MENU_LINE_22[] PROGMEM = "     TIME LIMIT    ";
const char MENU_LINE_23[] PROGMEM = "                   ";

const char* const upperDisplay[] PROGMEM = { MENU_LINE_0, MENU_LINE_4, MENU_LINE_8, MENU_LINE_12, MENU_LINE_16, MENU_LINE_20 };
const char* const upperMidDisplay[] PROGMEM = { MENU_LINE_1, MENU_LINE_5, MENU_LINE_9, MENU_LINE_13, MENU_LINE_17, MENU_LINE_21 };
const char* const lowerMidDisplay[] PROGMEM = { MENU_LINE_2, MENU_LINE_6, MENU_LINE_10, MENU_LINE_14, MENU_LINE_18, MENU_LINE_22 };
const char* const lowerDisplay[] PROGMEM = { MENU_LINE_3, MENU_LINE_7, MENU_LINE_11, MENU_LINE_15, MENU_LINE_19, MENU_LINE_23 };
