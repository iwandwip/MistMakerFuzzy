/*
 *  float-sens.cpp
 *
 *  float sensor c
 *  Created on: 2023. 4. 3
 */

#include "float-sens.h"

#include "Arduino.h"
#include "SimpleKalmanFilter.h"

#define SENSOR_FILTER_KF 8

FloatSensor::FloatSensor()
  : sensorPin(A0) {
        isCalibrate = false;
        floatValue = (isCalibrate) ? new int[SENS_RET_TOTAL_DATA] : new int;
        if (isCalibrate) cal_tm = new uint32_t;
}

FloatSensor::FloatSensor(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
        floatValue = (isCalibrate) ? new int[SENS_RET_TOTAL_DATA] : new int;
        if (isCalibrate) cal_tm = new uint32_t;
}

FloatSensor::~FloatSensor() {
}

void FloatSensor::init() {
        pinMode(sensorPin, INPUT);
}

void FloatSensor::update() {
        if (millis() - update_tm >= 50) {
                if (!isCalibrate) {
                        if (sensorPin < 14) {
                                *floatValue = !digitalRead(sensorPin);
                        } else {
                                *floatValue = !(analogRead(sensorPin) > 512) ? 1 : 0;
                        }

                        // *floatValue = *floatValue * (5.0 / 1023.0);
                        // *floatValue = *floatValue + (*floatValue * SENSOR_FILTER_KF);
                        // *floatValue /= SENSOR_FILTER_KF + 1;
                } else {
                        // SimpleKalmanFilter* sonarKf = new SimpleKalmanFilter(2, 2, 0.01);
                        // floatValue[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        // floatValue[SENS_RET_ACT_DATA] = floatValue[SENS_RET_RAW_DATA] + (floatValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        // floatValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        // floatValue[SENS_RET_AVG_DATA] = getSensorAverage(floatValue[SENS_RET_ACT_DATA]);
                        // // floatValue[SENS_RET_FILTERED_DATA] = lowPassFilter(floatValue[SENS_RET_ACT_DATA], floatValue[SENS_RET_FILTERED_DATA], 0.1);
                        // floatValue[SENS_RET_FILTERED_DATA] = sonarKf->updateEstimate(floatValue[SENS_RET_ACT_DATA]);
                        // delete sonarKf;
                }

                update_tm = millis();
        }
}

void FloatSensor::debug() {
        if (millis() - debug_tm >= 500) {
                if (isCalibrate) return;
                Serial.print("| floatValueRaw: ");
                Serial.print(*floatValue);
                Serial.println();
                debug_tm = millis();
        }
}

void FloatSensor::calibrate() {
        if (millis() - *cal_tm >= 500) {
                if (!isCalibrate) return;
                Serial.print("| floatValueRaw: ");
                Serial.print(floatValue[SENS_RET_RAW_DATA]);
                Serial.print("| floatValueAct: ");
                Serial.print(floatValue[SENS_RET_ACT_DATA]);
                Serial.print("| floatValueAvg: ");
                Serial.print(floatValue[SENS_RET_AVG_DATA]);
                Serial.print("| floatValueFiltered: ");
                Serial.print(floatValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                *cal_tm = millis();
        }
}

void FloatSensor::getSensorValue(float* value) {
}

void FloatSensor::getSensorValue(int* value) {
        *value = *floatValue;
}

void FloatSensor::getSensorValue(char* value) {
}

void FloatSensor::setCallBack(void (*callbackFunc)(void)) {
        thisCallbackFunc = callbackFunc;
}

void FloatSensor::count() {
}

void FloatSensor::reset() {
}

float FloatSensor::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return *floatValue;
        return floatValue[c];
}

void FloatSensor::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float FloatSensor::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++) {
                sensorSum += sensorValue;
        }
        return sensorSum / (float)numReadings;
}

float FloatSensor::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}