/*
 *  keypad-i2c.h
 *
 *  keypad i2c lib
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef KEYPADI2C_H
#define KEYPADI2C_H

#include "Arduino.h"
#include "Keypad.h"
#include "Keypad_I2C.h"
#include "Wire.h"
#include "sensor-module.h"
#include "stdint.h"

class KeypadI2C : public BaseSens {
       private:
        void (*thisCallbackFunc)(void) = nullptr;
        Keypad_I2C* keypadClass;
        uint8_t sensorPin;
        char* keyValue;

        uint32_t update_tm;
        uint32_t debug_tm;
        uint32_t* cal_tm;

        bool isCalibrate;

        float getSensorAverage(float sensorValue, int numReadings = 10);
        float lowPassFilter(float input, float output, float alpha);

       public:
        KeypadI2C();
        KeypadI2C(uint8_t __pin, bool enableCalibrate = false);
        ~KeypadI2C();

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
        void setPins(uint8_t __pin);
};

#endif  // KEYPAD_I2C_H