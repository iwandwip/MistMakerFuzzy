#include "Arduino.h"
#include "lcd-menu.h"
#include "keypad-i2c.h"
#include "serial-com.h"
#include "MemoryFree.h"
#include "float-sensor.h"
#include "sensor-module.h"
#include "max31865-sensor.h"
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
SensorModule sensor;
SerialCom com;

float temperatureValue;
char keypadValue, dummyValue;
int floatUpValue, floatDownValue;

String spData;
int isStart = 0;

void setup() {
        Serial.begin(9600);

        lcd.init();
        lcd.backlight();

        lcd.setCursor(6, 1);
        lcd.print("SYSTEMS");
        lcd.setCursor(5, 2);
        lcd.print("INITIALIZE");
}

void loop() {
        sensor.loop(routine);
        com.receive(onReceive);

        if (dummyValue == 'C') isStart = 0;
        if (!isStart) {
                if (!floatUpValue && !floatDownValue) {
                        for (uint8_t i = 0; i < 4; i++) {
                                lcd.setCursor(0, i);
                                lcd.print(menuLine[0][i]);
                        }
                } else if (!floatUpValue && floatDownValue) {
                        for (uint8_t i = 0; i < 4; i++) {
                                lcd.setCursor(0, i);
                                lcd.print(menuLine[1][i]);
                        }
                } else if (floatUpValue && floatDownValue) {
                        if (spData.length() < 4 && isDigit(dummyValue))
                                spData += String(dummyValue);
                        if (dummyValue == '*') spData = "";
                        if (dummyValue == '#' && spData != "") {
                                isStart = 1;
                                for (uint8_t i = 0; i < 4; i++) {
                                        lcd.setCursor(0, i);
                                        lcd.print(menuLine[3][i]);
                                }
                                delay(2000);
                        }
                        for (uint8_t i = 0; i < 4; i++) {
                                lcd.setCursor(0, i);
                                if (i == 2) {
                                        lcd.print("  SP:");
                                        lcd.print(spData + " *C");
                                        lcd.print(BLANK8);
                                } else {
                                        lcd.print(menuLine[2][i]);
                                }
                        }
                        dummyValue = '\0';
                }
        } else {
                for (uint8_t i = 0; i < 4; i++) {
                        lcd.setCursor(0, i);
                        if (i == 1) {
                                lcd.print("  SP:");
                                lcd.print(spData + " *C");
                                lcd.print(BLANK8);
                        } else if (i == 2) {
                                lcd.print("  AV:");
                                lcd.print("28.8 *C");
                                lcd.print(BLANK8);
                        } else {
                                lcd.print(menuLine[4][i]);
                        }
                }
        }

        Serial.print("| floatUp: ");
        Serial.print(floatUpValue);
        Serial.print("| floatDown: ");
        Serial.print(floatDownValue);
        Serial.print("| temperature: ");
        Serial.print(temperatureValue);
        Serial.print("| keypad: ");
        Serial.print(keypadValue);
        Serial.print("| freeMemory: ");
        Serial.print(freeMemory());
        Serial.println();
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
}

void onReceive(String data) {
        dummyValue = data.charAt(0);
}
