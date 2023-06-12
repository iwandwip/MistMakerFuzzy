/*
 *  sensor-module.h
 *
 *  sensor module header
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include "Arduino.h"
#include "HardwareSerial.h"
#include "SPI.h"

typedef enum {
        SENS_RET_RAW_DATA,
        SENS_RET_FILTERED_DATA,
        SENS_RET_OFFSET_DATA,
        SENS_RET_ACT_DATA,
        SENS_RET_AVG_DATA,
        SENS_RET_TOTAL_DATA,
} sens_ret_index_t;

class BaseSens {
      public:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void debug() = 0;
        virtual void calibrate() = 0;
        virtual void getValue(float* output) = 0;
        virtual void getValue(int* output) = 0;
        virtual void getValue(char* output) = 0;
        virtual void setCallBack(void (*callbackFunc)(void)) = 0;
        virtual void count() = 0;
        virtual void reset() = 0;

        BaseSens& operator=(const BaseSens&) = default;
        BaseSens& operator=(BaseSens&&) = default;
};

class SensorModule {
      private:
        BaseSens** base;
        uint8_t len = 0;
      public:
        SensorModule();
        ~SensorModule();

        void init(void (*initialize)(void) = nullptr);
        void update(void (*update)(void) = nullptr);
        void loop(void (*loop)(void) = nullptr);
        void debug(int __index = -1);

        void addModule(BaseSens* sensModule);
        void removeModule(uint8_t index);
        BaseSens* getModule(uint8_t index);
        void clearModules();
        uint8_t getModuleCount();

        void setModule(uint8_t index, BaseSens* sensModule);
        void swapModules(uint8_t index1, uint8_t index2);
        bool isModulePresent(BaseSens* sensModule);
        bool isModulePresent(uint8_t index);
};

// for excel (PLX-DAQ) / LabView
class SensorDebug {
      private:
        String label;
        String data;
        char* separator;
        uint8_t labelCount;
        uint8_t dataCount;
        uint32_t send_tm;
        bool send2Sheet;

      public:
        SensorDebug();
        void init(bool isSendToSheet = true);
        void addLabel(String _label, bool with_separator = true);
        template<typename T>
        void addData(T value, bool with_separator = true);
        void sendLabel();
        void sendData(uint32_t __t = 1000);
        void clearLabel();
        void clearData();
        void setLabel(String _label);
        void setData(String _data);
        void setSeparator(char _sep[]);
        void setSendDataCallback(void (*sendCallback)(void));
        String getLabel();
        String getData();
        void reset();
};


#endif  // SENSOR_MODULE_H