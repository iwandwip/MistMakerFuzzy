/*
 *  max31865-sensor.h
 *
 *  max31865 sensor lib
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef MAX31865_PT100_H
#define MAX31865_PT100_H

#include "Arduino.h"
#include "stdint.h"
#include "sensor-module.h"
#include "Adafruit_MAX31865.h"

class Max31865Pt100 : public BaseSens {
      private:
        Adafruit_MAX31865* sensorClass;
        float arrTemperatureValue[6];
        float temperatureValue;
        uint32_t sensTimer[3];

        bool isCalibrate;
        uint8_t sensorPin[4];

        void (*thisCallbackFunc)(void) = nullptr;
        float getSensorAverage(float sensorValue, int numReadings = 10);
        float lowPassFilter(float input, float output, float alpha);
      public:
        Max31865Pt100();
        Max31865Pt100(uint8_t _cs_pin, uint8_t _di_pin, uint8_t _do_pin, uint8_t _clk_pin, bool enableCalibrate = false);
        ~Max31865Pt100();

        void init() override;
        void update() override;
        void debug() override;
        void calibrate() override;

        void getValue(float* output) override;
        void getValue(int* output) override;
        void getValue(char* output) override;

        void setCallBack(void (*callbackFunc)(void)) override;
        void count() override;
        void reset() override;

        float getValue(sens_ret_index_t c = SENS_RET_ACT_DATA);
        void setPins(uint8_t _cs_pin, uint8_t _di_pin, uint8_t _do_pin, uint8_t _clk_pin);
};

#endif  // MAX31865_PT100_H