#include "Arduino.h"
#include "lcd-menu.h"
#include "keypad-i2c.h"
#include "serial-com.h"
#include "MemoryFree.h"
#include "float-sensor.h"
#include "sensor-module.h"
#include "timer-arduino.h"
#include "max31865-sensor.h"
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
SensorModule sensor;
TimerArduino timer;

const float temperatureSetpoint = 100;
int floatUpValue, floatDownValue;
float temperatureValue;
char keypadValue;

int taskSequence = 0;
uint32_t taskTimer;

char upperDisplayBuffer[25];
char upperMidDisplayBuffer[25];
char lowerMidDisplayBuffer[25];
char lowerDisplayBuffer[25];

char bufferSetTime[5] = "0";
uint8_t bufferSetTimeIndex = 0;


void setup() {
        Serial.begin(9600);
        lcd.init();
        lcd.backlight();
}

void loop() {
        sensor.loop(routine);
        // debug();
        if (keypadValue != NO_KEY && keypadValue == 'C') {
                taskSequence = 0;
                timer.reset();
                bufferSetTimeIndex = 0;
                strcpy(bufferSetTime, "0");
        }
        if (taskSequence == 0) {
                if (!floatUpValue && floatDownValue) taskSequence = 1;
                else if (floatUpValue && floatDownValue) taskSequence = 2;
        } else if (taskSequence == 1) {
                if (!floatUpValue && !floatDownValue) taskSequence = 0;
                if (floatUpValue && floatDownValue) taskSequence = 2;
        } else if (taskSequence == 2) {
                if (keypadValue != NO_KEY) {
                        if (keypadValue == '*') {
                                bufferSetTimeIndex = 0;
                                strcpy(bufferSetTime, "0");
                        }
                        if (keypadValue == '#' && atoi(bufferSetTime) != 0) {
                                taskSequence = 3;
                                timer.setDuration(atoi(bufferSetTime) * 60);
                                taskTimer = millis();
                        }
                        if (bufferSetTimeIndex < 3 && isDigit(keypadValue)) {
                                bufferSetTime[bufferSetTimeIndex++] = keypadValue;
                                bufferSetTime[bufferSetTimeIndex] = '\0';
                        }
                }
                sprintf(lowerMidDisplayBuffer, "  TIME: %3s Minute   ", bufferSetTime);
        } else if (taskSequence == 3) {
                if (millis() - taskTimer >= 1000) {
                        taskSequence = 4;
                        timer.start();
                        taskTimer = millis();
                }
        } else if (taskSequence == 4) {
                char strTemperatureSetpoint[20], strPVTemperature[20];
                dtostrf(temperatureValue, 4, 2, strPVTemperature);
                dtostrf(temperatureSetpoint, 4, 2, strTemperatureSetpoint);

                sprintf(upperDisplayBuffer, "  Elapsed %02d:%02d:%02d  ", timer.getHours(), timer.getMinutes(), timer.getSeconds());
                sprintf(upperMidDisplayBuffer, "  SV: %6s *C  ", strTemperatureSetpoint);
                sprintf(lowerMidDisplayBuffer, "  PV: %6s *C  ", strPVTemperature);

                if (timer.isExpired()) {
                        taskSequence = 5;
                        timer.reset();
                        taskTimer = millis();
                        bufferSetTimeIndex = 0;
                        strcpy(bufferSetTime, "0");
                }
        } else if (taskSequence == 5) {
                if (millis() - taskTimer >= 1000) {
                        taskSequence = 0;
                        taskTimer = millis();
                }
        }
        display();
}

void routine() {
        sensor.addModule(new FloatSensor(A7));
        sensor.addModule(new FloatSensor(2));
        sensor.addModule(new Max31865Pt100(10, 11, 12, 13));
        sensor.addModule(new KeypadI2C());

        sensor.init();
        sensor.update();

        sensor.getModule(0)->getValue(&floatUpValue);
        sensor.getModule(1)->getValue(&floatDownValue);
        sensor.getModule(2)->getValue(&temperatureValue);
        sensor.getModule(3)->getValue(&keypadValue);

        sensor.clearModules();

        strcpy_P(upperDisplayBuffer, (char*)pgm_read_word(&(upperDisplay[taskSequence])));
        strcpy_P(upperMidDisplayBuffer, (char*)pgm_read_word(&(upperMidDisplay[taskSequence])));
        strcpy_P(lowerMidDisplayBuffer, (char*)pgm_read_word(&(lowerMidDisplay[taskSequence])));
        strcpy_P(lowerDisplayBuffer, (char*)pgm_read_word(&(lowerDisplay[taskSequence])));
}

void display() {
        lcd.setCursor(0, 0);
        lcd.print(upperDisplayBuffer);
        lcd.setCursor(0, 1);
        lcd.print(upperMidDisplayBuffer);
        lcd.setCursor(0, 2);
        lcd.print(lowerMidDisplayBuffer);
        lcd.setCursor(0, 3);
        lcd.print(lowerDisplayBuffer);
}

void debug() {
        Serial.print("| floatUp: ");
        Serial.print(floatUpValue);
        Serial.print("| floatDown: ");
        Serial.print(floatDownValue);
        Serial.print("| temperature: ");
        Serial.print(temperatureValue);
        Serial.print("| keypad: ");
        Serial.print(keypadValue);
        Serial.print("| time: ");
        Serial.print(atoi(bufferSetTime));
        Serial.print("| freeMemory: ");
        Serial.print(freeMemory());
        Serial.println();
}
