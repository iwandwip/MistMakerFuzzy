/*
 *  abstract-sens.h
 *
 *  abstract sensor lib
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef ABSTRACT_SENS_H
#define ABSTRACT_SENS_H

#include "Adafruit_MAX31865.h"
#include "Arduino.h"
#include "sensor-module.h"
#include "stdint.h"

class MAX31865_PT100 : public BaseSens {
       private:
        void (*thisCallbackFunc)(void) = nullptr;
        Adafruit_MAX31865* sensorClass;
        uint8_t sensorPin[4];
        float* temperature;

        uint32_t update_tm;
        uint32_t debug_tm;
        uint32_t* cal_tm;

        bool isCalibrate;

        float getSensorAverage(float sensorValue, int numReadings = 10);
        float lowPassFilter(float input, float output, float alpha);

       public:
        MAX31865_PT100();
        MAX31865_PT100(uint8_t _cs_pin, uint8_t _di_pin, uint8_t _do_pin, uint8_t _clk_pin, bool enableCalibrate = false);
        ~MAX31865_PT100();

        void init() override;
        void update() override;
        void debug() override;
        void calibrate() override;
        void getSensorValue(float* value) override;
        void getSensorValue(int* value) override;
        void getSensorValue(char* value) override;
        void setCallBack(void (*callbackFunc)(void)) override;
        void count() override;
        void reset() override;

        float getValue(sens_ret_index_t c = SENS_RET_ACT_DATA);
        void setPins(uint8_t _cs_pin, uint8_t _di_pin, uint8_t _do_pin, uint8_t _clk_pin);
};

#endif  // ABSTRACT_SENS_H