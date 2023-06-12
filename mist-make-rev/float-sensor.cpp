/*
 *  float-sensor.cpp
 *
 *  float sensor c
 *  Created on: 2023. 4. 3
 */

#include "float-sensor.h"
#include "Arduino.h"
#include "sensor-filter.h"

#define SENSOR_FILTER_KF 8

FloatSensor::FloatSensor()
  : sensorPin(A0), isCalibrate(false) {
}

FloatSensor::FloatSensor(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
}

FloatSensor::~FloatSensor() {
        delete this;
}

void FloatSensor::init() {
        pinMode(sensorPin, INPUT);
}

void FloatSensor::update() {
        if (millis() - sensTimer[0] >= 30) {
                if (!isCalibrate) {
                        if (sensorPin < 14) floatSensorValue = !digitalRead(sensorPin);
                        else floatSensorValue = !(analogRead(sensorPin) > 512) ? 1 : 0;
                } else {
                        KalmanFilter* sensorKf = new KalmanFilter(2, 2, 0.01);
                        arrFloatSensorValue[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        arrFloatSensorValue[SENS_RET_ACT_DATA] = arrFloatSensorValue[SENS_RET_RAW_DATA] + (arrFloatSensorValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        arrFloatSensorValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        arrFloatSensorValue[SENS_RET_AVG_DATA] = getSensorAverage(arrFloatSensorValue[SENS_RET_ACT_DATA]);
                        // arrFloatSensorValue[SENS_RET_FILTERED_DATA] = lowPassFilter(arrFloatSensorValue[SENS_RET_ACT_DATA], arrFloatSensorValue[SENS_RET_FILTERED_DATA], 0.1);
                        arrFloatSensorValue[SENS_RET_FILTERED_DATA] = sensorKf->updateEstimate(arrFloatSensorValue[SENS_RET_ACT_DATA]);
                        delete sensorKf;
                }
                sensTimer[0] = millis();
        }
}

void FloatSensor::debug() {
        if (millis() - sensTimer[1] >= 500) {
                if (isCalibrate) return;
                Serial.print("| floatSensorValueRaw: ");
                Serial.print(floatSensorValue);
                Serial.println();
                sensTimer[1] = millis();
        }
}

void FloatSensor::calibrate() {
        if (millis() - sensTimer[2] >= 500) {
                if (!isCalibrate) return;
                Serial.print("| arrFloatSensorValueRaw: ");
                Serial.print(arrFloatSensorValue[SENS_RET_RAW_DATA]);
                Serial.print("| arrFloatSensorValueAct: ");
                Serial.print(arrFloatSensorValue[SENS_RET_ACT_DATA]);
                Serial.print("| arrFloatSensorValueAvg: ");
                Serial.print(arrFloatSensorValue[SENS_RET_AVG_DATA]);
                Serial.print("| arrFloatSensorValueFiltered: ");
                Serial.print(arrFloatSensorValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                sensTimer[2] = millis();
        }
}

void FloatSensor::getValue(float* output) {
}

void FloatSensor::getValue(int* output) {
        *output = floatSensorValue;
}

void FloatSensor::getValue(char* output) {
}

void FloatSensor::setCallBack(void (*callbackFunc)(void)) {
        thisCallbackFunc = callbackFunc;
}

void FloatSensor::count() {
}

void FloatSensor::reset() {
}

float FloatSensor::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return floatSensorValue;
        return arrFloatSensorValue[c];
}

void FloatSensor::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float FloatSensor::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++)
                sensorSum += sensorValue;
        return sensorSum / (float)numReadings;
}

float FloatSensor::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}