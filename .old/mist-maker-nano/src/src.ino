#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "MAX31865-PT100.h"
#include "Wire.h"
#include "float-sens.h"
#include "keypad-i2c.h"
// #include "output-module.h"
#include "sensor-module.h"
#include "serial-com.h"
#include "lcd-menu.h"

#include "MemoryFree.h"

void sensorRoutine();
void onReceive(String data);

LiquidCrystal_I2C lcd(0x27, 20, 4);

SensorModule sensor;
float temperature;
int floatValue[2];
char keyValue;
char dummyKey;

SerialCom com;

// DigitalOut ssrWaterHeater(9);
// DigitalOut relayPilotLampA(A0);
// DigitalOut relayPilotLampB(A1);
// DigitalOut relayPilotLampC(A2);
// DigitalOut relayPilotLampD(A3);

void setup() {
        Serial.begin(9600);
        com.begin(9600);
        sensor.addModule(new MAX31865_PT100(10, 11, 12, 13));
        sensor.addModule(new FloatSensor(A7));
        sensor.addModule(new FloatSensor(2));
        sensor.addModule(new KeypadI2C());
        sensor.init();

        lcd.init();
        lcd.backlight();

        lcd.setCursor(6, 1);
        lcd.print("SYSTEMS");
        lcd.setCursor(5, 2);
        lcd.print("INITIALIZE");
}

void loop() {
        com.receive(onReceive);
        sensor.update(sensorRoutine);

        static String spData;
        static int isStart = 0;

        if (!isStart) {
                if (!floatValue[1] && !floatValue[0]) {
                        for (uint8_t i = 0; i < 4; i++) {
                                lcd.setCursor(0, i);
                                lcd.print(menuLine[0][i]);
                        }
                } else if (!floatValue[1] && floatValue[0]) {
                        for (uint8_t i = 0; i < 4; i++) {
                                lcd.setCursor(0, i);
                                lcd.print(menuLine[1][i]);
                        }
                } else if (floatValue[1] && floatValue[0]) {
                        if (spData.length() < 4 && isDigit(dummyKey))
                                spData += String(dummyKey);
                        if (dummyKey == '*') spData = "";
                        if (dummyKey == '#' && spData != "") {
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
                        dummyKey = '\0';
                }
        } else {
                lcd.clear();
        }


        Serial.print("| temp: ");
        Serial.print(temperature);
        Serial.print("| floatUp: ");
        Serial.print(floatValue[0]);
        Serial.print("| floatDown: ");
        Serial.print(floatValue[1]);
        Serial.print("| keyPad: ");
        Serial.print(dummyKey);
        Serial.print("| freeMemory: ");
        Serial.print(freeMemory());
        Serial.println();
}

void sensorRoutine() {
        sensor.getModule(0)->getSensorValue(&temperature);
        sensor.getModule(1)->getSensorValue(&floatValue[0]);
        sensor.getModule(2)->getSensorValue(&floatValue[1]);
        sensor.getModule(3)->getSensorValue(&keyValue);
}

void onReceive(String data) {
        dummyKey = data.charAt(0);
}