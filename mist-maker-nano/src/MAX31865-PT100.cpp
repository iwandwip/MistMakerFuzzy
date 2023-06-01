/*
 *  abstract-sens.cpp
 *
 *  abstract sensor c
 *  Created on: 2023. 4. 3
 */

#include "MAX31865-PT100.h"

#include "Arduino.h"
#include "SimpleKalmanFilter.h"

#define SENSOR_FILTER_KF 2
#define RREF 430.0
#define RNOMINAL 100.0

MAX31865_PT100::MAX31865_PT100() {
        isCalibrate = false;
        temperature = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
}

MAX31865_PT100::MAX31865_PT100(uint8_t _cs_pin, uint8_t _di_pin, uint8_t _do_pin, uint8_t _clk_pin, bool enableCalibrate) {
        this->sensorPin[0] = _cs_pin;
        this->sensorPin[1] = _di_pin;
        this->sensorPin[2] = _do_pin;
        this->sensorPin[3] = _clk_pin;
        isCalibrate = enableCalibrate;
        temperature = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
}

MAX31865_PT100::~MAX31865_PT100() {
}

void MAX31865_PT100::init() {
        sensorClass = new Adafruit_MAX31865(sensorPin[0], sensorPin[1], sensorPin[2], sensorPin[3]);
        sensorClass->begin(MAX31865_3WIRE);
}

void MAX31865_PT100::update() {
        if (millis() - update_tm >= 500) {
                if (!isCalibrate) {
                        // uint16_t rtd = sensorClass->readRTD();
                        *temperature = (float)sensorClass->temperature(RNOMINAL, RREF);
                        *temperature = *temperature + (*temperature * SENSOR_FILTER_KF);
                        *temperature /= SENSOR_FILTER_KF + 1;
                } else {
                        SimpleKalmanFilter* sonarKf = new SimpleKalmanFilter(2, 2, 0.01);
                        temperature[SENS_RET_RAW_DATA] = (float)sensorClass->temperature(RNOMINAL, RREF);
                        temperature[SENS_RET_ACT_DATA] = temperature[SENS_RET_RAW_DATA] + (temperature[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        temperature[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        temperature[SENS_RET_AVG_DATA] = getSensorAverage(temperature[SENS_RET_ACT_DATA]);
                        // temperature[SENS_RET_FILTERED_DATA] = lowPassFilter(temperature[SENS_RET_ACT_DATA], temperature[SENS_RET_FILTERED_DATA], 0.1);
                        temperature[SENS_RET_FILTERED_DATA] = sonarKf->updateEstimate(temperature[SENS_RET_ACT_DATA]);
                        delete sonarKf;
                }

                update_tm = millis();
        }
}

void MAX31865_PT100::debug() {
        if (millis() - debug_tm >= 500) {
                if (isCalibrate) return;
                Serial.print("| temperatureRaw: ");
                Serial.print(*temperature);
                Serial.println();
                debug_tm = millis();
        }
}

void MAX31865_PT100::calibrate() {
        if (millis() - *cal_tm >= 500) {
                if (!isCalibrate) return;
                Serial.print("| temperatureRaw: ");
                Serial.print(temperature[SENS_RET_RAW_DATA]);
                Serial.print("| temperatureAct: ");
                Serial.print(temperature[SENS_RET_ACT_DATA]);
                Serial.print("| temperatureAvg: ");
                Serial.print(temperature[SENS_RET_AVG_DATA]);
                Serial.print("| temperatureFiltered: ");
                Serial.print(temperature[SENS_RET_FILTERED_DATA]);
                Serial.println();
                *cal_tm = millis();
        }
}

void MAX31865_PT100::getSensorValue(float* value) {
        *value = *temperature;
}

void MAX31865_PT100::getSensorValue(int* value) {
}

void MAX31865_PT100::getSensorValue(char* value) {
}

void MAX31865_PT100::setCallBack(void (*callbackFunc)(void)) {
        thisCallbackFunc = callbackFunc;
}

void MAX31865_PT100::count() {
}

void MAX31865_PT100::reset() {
}

float MAX31865_PT100::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return *temperature;
        return temperature[c];
}

void MAX31865_PT100::setPins(uint8_t _cs_pin, uint8_t _di_pin, uint8_t _do_pin, uint8_t _clk_pin) {
        this->sensorPin[0] = _cs_pin;
        this->sensorPin[1] = _di_pin;
        this->sensorPin[2] = _do_pin;
        this->sensorPin[3] = _clk_pin;
}

float MAX31865_PT100::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++) {
                sensorSum += sensorValue;
        }
        return sensorSum / (float)numReadings;
}

float MAX31865_PT100::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}