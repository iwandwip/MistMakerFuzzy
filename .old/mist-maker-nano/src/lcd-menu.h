/*
 *  lcd-menu.h
 *
 *  lcd menu h
 *  Created on: 2023. 4. 3
 */

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

const char* menuLine[5][4] = { {
                                 "                    ",
                                 "     TANK KOSONG    ",
                                 "      ISI TANK      ",
                                 "                    ",
                               },
                               {
                                 "                    ",
                                 "  TANK BELUM PENUH  ",
                                 "      ISI TANK      ",
                                 "                    ",
                               },
                               {
                                 "    SISTEM READY    ",
                                 "  MASUKAN SETPOINT  ",
                                 "  SP:               ",
                                 "  * CLEAR | # SET    ",
                               },
                               {
                                 "                    ",
                                 "    SISTEM START    ",
                                 "      WAIT ...      ",
                                 "                    ",
                               },
                               {
                                 "     ON PROGRESS    ",
                                 "  SP:               ",
                                 "  AV:               ",
                                 "  C for Interrupt   ",
                               } };