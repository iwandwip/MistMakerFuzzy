/*
 *  lcd-menu.h
 *
 *  lcd menu h
 *  Created on: 2023. 4. 3
 */

#include "Arduino.h"

#define BLANK0 ""
#define BLANK1 " "
#define BLANK2 "  "
#define BLANK3 "   "
#define BLANK4 "    "
#define BLANK5 "     "
#define BLANK6 "      "
#define BLANK7 "       "
#define BLANK8 "        "
#define BLANK9 "         "
#define BLANK10 "          "

#define MENU_LINE_0 "                    "
#define MENU_LINE_1 "     TANK KOSONG    "
#define MENU_LINE_2 "      ISI TANK      "
#define MENU_LINE_3 "                    "

#define MENU_LINE_4 "                    "
#define MENU_LINE_5 "  TANK BELUM PENUH  "
#define MENU_LINE_6 "      ISI TANK      "
#define MENU_LINE_7 "                    "

#define MENU_LINE_8 "    SISTEM READY    "
#define MENU_LINE_9 "  MASUKAN SETPOINT  "
#define MENU_LINE_10 "  SP:               "
#define MENU_LINE_11 "  * CLEAR | # SET    "

#define MENU_LINE_12 "                    "
#define MENU_LINE_13 "    SISTEM START    "
#define MENU_LINE_14 "      WAIT ...      "
#define MENU_LINE_15 "                    "

#define MENU_LINE_16 "     ON PROGRESS    "
#define MENU_LINE_17 "  SP:               "
#define MENU_LINE_18 "  AV:               "
#define MENU_LINE_19 "  C for Interrupt   "

const char* menuLine[5][4] = {
        { MENU_LINE_0, MENU_LINE_1, MENU_LINE_2, MENU_LINE_3 },
        { MENU_LINE_4, MENU_LINE_5, MENU_LINE_6, MENU_LINE_7 },
        { MENU_LINE_8, MENU_LINE_9, MENU_LINE_10, MENU_LINE_11 },
        { MENU_LINE_12, MENU_LINE_13, MENU_LINE_14, MENU_LINE_15 },
        { MENU_LINE_16, MENU_LINE_17, MENU_LINE_18, MENU_LINE_19 }
};