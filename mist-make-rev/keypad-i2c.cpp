/*
 *  keypad-i2c.cpp
 *
 *  keypad i2c c
 *  Created on: 2023. 4. 3
 */


#include "keypad-i2c.h"
#include "Arduino.h"
#include "sensor-filter.h"

#define SENSOR_FILTER_KF 8
#define I2CADDR 0x38

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
        { '1', '2', '3', 'A' },
        { '4', '5', '6', 'B' },
        { '7', '8', '9', 'C' },
        { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 3, 2, 1, 0 };
byte colPins[COLS] = { 7, 6, 5, 4 };

KeypadI2C::KeypadI2C()
  : sensorPin(A0), isCalibrate(false) {
}

KeypadI2C::KeypadI2C(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
}

KeypadI2C::~KeypadI2C() {
        delete this;
}

void KeypadI2C::init() {
        sensorClass = new Keypad_I2C(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR);
        Wire.begin();
        sensorClass->begin();
}

void KeypadI2C::update() {
        if (millis() - sensTimer[0] >= 50) {
                if (!isCalibrate) {
                        keypadValue = sensorClass->getKey();
                } else {
                        KalmanFilter* sensorKf = new KalmanFilter(2, 2, 0.01);
                        arrKeypadValue[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        arrKeypadValue[SENS_RET_ACT_DATA] = arrKeypadValue[SENS_RET_RAW_DATA] + (arrKeypadValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        arrKeypadValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        arrKeypadValue[SENS_RET_AVG_DATA] = getSensorAverage(arrKeypadValue[SENS_RET_ACT_DATA]);
                        // arrKeypadValue[SENS_RET_FILTERED_DATA] = lowPassFilter(arrKeypadValue[SENS_RET_ACT_DATA], arrKeypadValue[SENS_RET_FILTERED_DATA], 0.1);
                        arrKeypadValue[SENS_RET_FILTERED_DATA] = sensorKf->updateEstimate(arrKeypadValue[SENS_RET_ACT_DATA]);
                        delete sensorKf;
                }
                sensTimer[0] = millis();
        }
        Wire.end();
        delete sensorClass;
        sensorClass = nullptr;
}

void KeypadI2C::debug() {
        if (millis() - sensTimer[1] >= 500) {
                if (isCalibrate) return;
                Serial.print("| keypadValueRaw: ");
                Serial.print(keypadValue);
                Serial.println();
                sensTimer[1] = millis();
        }
}

void KeypadI2C::calibrate() {
        if (millis() - sensTimer[2] >= 500) {
                if (!isCalibrate) return;
                Serial.print("| arrKeypadValueRaw: ");
                Serial.print(arrKeypadValue[SENS_RET_RAW_DATA]);
                Serial.print("| arrKeypadValueAct: ");
                Serial.print(arrKeypadValue[SENS_RET_ACT_DATA]);
                Serial.print("| arrKeypadValueAvg: ");
                Serial.print(arrKeypadValue[SENS_RET_AVG_DATA]);
                Serial.print("| arrKeypadValueFiltered: ");
                Serial.print(arrKeypadValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                sensTimer[2] = millis();
        }
}

void KeypadI2C::getValue(float* output) {
}

void KeypadI2C::getValue(int* output) {
}

void KeypadI2C::getValue(char* output) {
        *output = keypadValue;
}

void KeypadI2C::setCallBack(void (*callbackFunc)(void)) {
        thisCallbackFunc = callbackFunc;
}

void KeypadI2C::count() {
}

void KeypadI2C::reset() {
}

float KeypadI2C::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return keypadValue;
        return arrKeypadValue[c];
}

void KeypadI2C::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float KeypadI2C::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++)
                sensorSum += sensorValue;
        return sensorSum / (float)numReadings;
}

float KeypadI2C::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}