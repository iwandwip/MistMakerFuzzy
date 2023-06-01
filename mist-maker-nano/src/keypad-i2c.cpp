/*
 *  keypad-i2c.cpp
 *
 *  keypad i2c c
 *  Created on: 2023. 4. 3
 */

#include "keypad-i2c.h"

#include "Arduino.h"
#include "SimpleKalmanFilter.h"

#define SENSOR_FILTER_KF 8

#define I2CADDR 0x21

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
    {'0', '1', '2', '3'},
    {'4', '5', '6', '7'},
    {'8', '9', 'A', 'B'},
    {'C', 'D', 'E', 'F'}};
byte rowPins[ROWS] = {3, 2, 1, 0};
byte colPins[COLS] = {7, 6, 5, 4};

KeypadI2C::KeypadI2C() {
        isCalibrate = false;
        keyValue = (isCalibrate) ? new char[SENS_RET_TOTAL_DATA] : new char;
        if (isCalibrate) cal_tm = new uint32_t;
}

KeypadI2C::KeypadI2C(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
        keyValue = (isCalibrate) ? new char[SENS_RET_TOTAL_DATA] : new char;
        if (isCalibrate) cal_tm = new uint32_t;
}

KeypadI2C::~KeypadI2C() {
}

void KeypadI2C::init() {
        keypadClass = new Keypad_I2C(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR);
        Wire.begin();
        keypadClass->begin();
}

void KeypadI2C::update() {
        if (millis() - update_tm >= 50) {
                if (!isCalibrate) {
                        *keyValue = keypadClass->getKey();
                } else {
                        // SimpleKalmanFilter* sonarKf = new SimpleKalmanFilter(2, 2, 0.01);
                        // keyValue[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        // keyValue[SENS_RET_ACT_DATA] = keyValue[SENS_RET_RAW_DATA] + (keyValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        // keyValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        // keyValue[SENS_RET_AVG_DATA] = getSensorAverage(keyValue[SENS_RET_ACT_DATA]);
                        // // keyValue[SENS_RET_FILTERED_DATA] = lowPassFilter(keyValue[SENS_RET_ACT_DATA], keyValue[SENS_RET_FILTERED_DATA], 0.1);
                        // keyValue[SENS_RET_FILTERED_DATA] = sonarKf->updateEstimate(keyValue[SENS_RET_ACT_DATA]);
                        // delete sonarKf;
                }

                update_tm = millis();
        }
}

void KeypadI2C::debug() {
        if (millis() - debug_tm >= 500) {
                if (isCalibrate) return;
                Serial.print("| keyValueRaw: ");
                Serial.print(*keyValue);
                Serial.println();
                debug_tm = millis();
        }
}

void KeypadI2C::calibrate() {
        if (millis() - *cal_tm >= 500) {
                if (!isCalibrate) return;
                Serial.print("| keyValueRaw: ");
                Serial.print(keyValue[SENS_RET_RAW_DATA]);
                Serial.print("| keyValueAct: ");
                Serial.print(keyValue[SENS_RET_ACT_DATA]);
                Serial.print("| keyValueAvg: ");
                Serial.print(keyValue[SENS_RET_AVG_DATA]);
                Serial.print("| keyValueFiltered: ");
                Serial.print(keyValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                *cal_tm = millis();
        }
}

void KeypadI2C::getSensorValue(float* value) {
}

void KeypadI2C::getSensorValue(int* value) {
}

void KeypadI2C::getSensorValue(char* value) {
        *value = *keyValue;
}

void KeypadI2C::setCallBack(void (*callbackFunc)(void)) {
        thisCallbackFunc = callbackFunc;
}

void KeypadI2C::count() {
}

void KeypadI2C::reset() {
}

float KeypadI2C::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return *keyValue;
        return keyValue[c];
}

void KeypadI2C::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float KeypadI2C::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++) {
                sensorSum += sensorValue;
        }
        return sensorSum / (float)numReadings;
}

float KeypadI2C::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}