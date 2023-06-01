#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "MAX31865-PT100.h"
#include "Wire.h"
#include "float-sens.h"
#include "input-module.h"
#include "keypad-i2c.h"
#include "output-module.h"
#include "sensor-module.h"
#include "timer-ticks.h"

void sensorRoutine();

LiquidCrystal_I2C lcd(0x27, 16, 2);

SensorModule sensor;
float temperature;
int floatValue[2];
char keyValue;

DigitalOut ssrWaterHeater(2);
DigitalOut relayPilotLampA(3);
DigitalOut relayPilotLampB(4);
DigitalOut relayPilotLampC(5);
DigitalOut relayPilotLampD(6);

void setup() {
        Serial.begin(9600);
        sensor.addModule(new MAX31865_PT100(2, 3, 4, 5));
        sensor.addModule(new FloatSensor(2));
        sensor.addModule(new FloatSensor(3));
        sensor.addModule(new KeypadI2C());
        sensor.init();
        lcd.init();
        lcd.backlight();
        lcd.print("Init");
}

void loop() {
        sensor.update(sensorRoutine);
}

void sensorRoutine() {
        sensor.getModule(0)->getSensorValue(&temperature);
        sensor.getModule(1)->getSensorValue(&floatValue[0]);
        sensor.getModule(2)->getSensorValue(&floatValue[1]);
        sensor.getModule(3)->getSensorValue(&keyValue);
}