/*
 *  max31865-sensor.cpp
 *
 *  max31865 sensor c
 *  Created on: 2023. 4. 3
 */

#include "max31865-sensor.h"
#include "Arduino.h"
#include "sensor-filter.h"

#define SENSOR_FILTER_KF 2
#define RREF 430.0
#define RNOMINAL 100.0

Max31865Pt100::Max31865Pt100()
  : isCalibrate(false) {
}

Max31865Pt100::Max31865Pt100(uint8_t _cs_pin, uint8_t _di_pin, uint8_t _do_pin, uint8_t _clk_pin, bool enableCalibrate) {
        this->sensorPin[0] = _cs_pin;
        this->sensorPin[1] = _di_pin;
        this->sensorPin[2] = _do_pin;
        this->sensorPin[3] = _clk_pin;
        isCalibrate = enableCalibrate;
}

Max31865Pt100::~Max31865Pt100() {
        delete this;
}

void Max31865Pt100::init() {
        sensorClass = new Adafruit_MAX31865(sensorPin[0], sensorPin[1], sensorPin[2], sensorPin[3]);
        sensorClass->begin(MAX31865_3WIRE);
}

void Max31865Pt100::update() {
        if (millis() - sensTimer[0] >= 50) {
                if (!isCalibrate) {
                        // uint16_t rtd = sensorClass->readRTD();
                        temperatureValue = (float)sensorClass->temperature(RNOMINAL, RREF);
                        temperatureValue = temperatureValue + (temperatureValue * SENSOR_FILTER_KF);
                        temperatureValue /= SENSOR_FILTER_KF + 1;
                } else {
                        KalmanFilter* sensorKf = new KalmanFilter(2, 2, 0.01);
                        arrTemperatureValue[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        arrTemperatureValue[SENS_RET_ACT_DATA] = arrTemperatureValue[SENS_RET_RAW_DATA] + (arrTemperatureValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        arrTemperatureValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        arrTemperatureValue[SENS_RET_AVG_DATA] = getSensorAverage(arrTemperatureValue[SENS_RET_ACT_DATA]);
                        // arrTemperatureValue[SENS_RET_FILTERED_DATA] = lowPassFilter(arrTemperatureValue[SENS_RET_ACT_DATA], arrTemperatureValue[SENS_RET_FILTERED_DATA], 0.1);
                        arrTemperatureValue[SENS_RET_FILTERED_DATA] = sensorKf->updateEstimate(arrTemperatureValue[SENS_RET_ACT_DATA]);
                        delete sensorKf;
                }
                sensTimer[0] = millis();
        }
        delete sensorClass;
        sensorClass = nullptr;
}

void Max31865Pt100::debug() {
        if (millis() - sensTimer[1] >= 500) {
                if (isCalibrate) return;
                Serial.print("| temperatureValueRaw: ");
                Serial.print(temperatureValue);
                Serial.println();
                sensTimer[1] = millis();
        }
}

void Max31865Pt100::calibrate() {
        if (millis() - sensTimer[2] >= 500) {
                if (!isCalibrate) return;
                Serial.print("| arrTemperatureValueRaw: ");
                Serial.print(arrTemperatureValue[SENS_RET_RAW_DATA]);
                Serial.print("| arrTemperatureValueAct: ");
                Serial.print(arrTemperatureValue[SENS_RET_ACT_DATA]);
                Serial.print("| arrTemperatureValueAvg: ");
                Serial.print(arrTemperatureValue[SENS_RET_AVG_DATA]);
                Serial.print("| arrTemperatureValueFiltered: ");
                Serial.print(arrTemperatureValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                sensTimer[2] = millis();
        }
}

void Max31865Pt100::getValue(float* output) {
        *output = temperatureValue;
}

void Max31865Pt100::getValue(int* output) {
}

void Max31865Pt100::getValue(char* output) {
}

void Max31865Pt100::setCallBack(void (*callbackFunc)(void)) {
        thisCallbackFunc = callbackFunc;
}

void Max31865Pt100::count() {
}

void Max31865Pt100::reset() {
}

float Max31865Pt100::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return temperatureValue;
        return arrTemperatureValue[c];
}

void Max31865Pt100::setPins(uint8_t _cs_pin, uint8_t _di_pin, uint8_t _do_pin, uint8_t _clk_pin) {
        this->sensorPin[0] = _cs_pin;
        this->sensorPin[1] = _di_pin;
        this->sensorPin[2] = _do_pin;
        this->sensorPin[3] = _clk_pin;
}

float Max31865Pt100::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++)
                sensorSum += sensorValue;
        return sensorSum / (float)numReadings;
}

float Max31865Pt100::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}