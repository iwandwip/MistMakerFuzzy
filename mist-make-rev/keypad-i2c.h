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
#include "Wire.h"
#include "stdint.h"
#include "Keypad.h"
#include "Keypad_I2C.h"
#include "sensor-module.h"

class KeypadI2C : public BaseSens {
      private:
        Keypad_I2C* sensorClass;
        char arrKeypadValue[6];
        char keypadValue;
        uint32_t sensTimer[3];

        bool isCalibrate;
        uint8_t sensorPin;

        void (*thisCallbackFunc)(void) = nullptr;
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

        void getValue(float* output) override;
        void getValue(int* output) override;
        void getValue(char* output) override;

        void setCallBack(void (*callbackFunc)(void)) override;
        void count() override;
        void reset() override;

        float getValue(sens_ret_index_t c = SENS_RET_ACT_DATA);
        void setPins(uint8_t __pin);
};

#endif  // KEYPADI2C_H