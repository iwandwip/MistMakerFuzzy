/*
 *  sensor-module.cpp
 *
 *  sensor module c
 *  Created on: 2023. 4. 3
 */

#include "sensor-module.h"

#include "Arduino.h"
#include "HardwareSerial.h"

SensorModule::SensorModule()
    : base(nullptr) {
}

SensorModule::~SensorModule() {
        if (base != nullptr) {
                free(base);
                base = nullptr;
                len = 0;
        }
}

void SensorModule::init(void (*initialize)(void)) {
        if (initialize != nullptr) initialize();
        if (base == nullptr) return;
        for (uint8_t i = 0; i < len; i++) {
                base[i]->init();
        }
}

void SensorModule::update(void (*update)(void)) {
        if (update != nullptr) update();
        if (base == nullptr) return;
        for (uint8_t i = 0; i < len; i++) {
                base[i]->update();
        }
}

void SensorModule::debug(int __index) {
        if (base == nullptr) return;
        if (__index != -1)
                base[__index]->debug();
        else {
                for (uint8_t i = 0; i < len; i++) {
                        base[i]->debug();
                }
        }
}

void SensorModule::addModule(BaseSens* sensModule) {
        BaseSens** newBase = (BaseSens**)realloc(base, (len + 1) * sizeof(BaseSens*));  // Increase length by 1
        if (newBase == nullptr) {
                while (1) Serial.println("Memory Allocation Failed !");
        }
        base = newBase;
        base[len] = sensModule;  // Assign to correct index
        len++;
}

void SensorModule::removeModule(uint8_t index) {
        if (base == nullptr || index >= len) return;
        delete base[index];
        for (uint8_t i = index; i < len - 1; i++) {
                base[i] = base[i + 1];
        }
        len--;
        BaseSens** newBase = (BaseSens**)realloc(base, len * sizeof(BaseSens*));
        if (newBase != nullptr) {
                base = newBase;
        }
}

BaseSens* SensorModule::getModule(uint8_t index) {
        if (base == nullptr || index >= len) return nullptr;
        return base[index];
}

void SensorModule::clearModules() {
        if (base == nullptr) return;
        for (uint8_t i = 0; i < len; i++) {
                delete base[i];
        }
        free(base);
        base = nullptr;
        len = 0;
}

uint8_t SensorModule::getModuleCount() {
        return len;
}

void SensorModule::setModule(uint8_t index, BaseSens* sensModule) {
        if (base == nullptr || index >= len) return;
        delete base[index];
        base[index] = sensModule;
}

void SensorModule::swapModules(uint8_t index1, uint8_t index2) {
        if (base == nullptr || index1 >= len || index2 >= len) return;
        BaseSens* temp = base[index1];
        base[index1] = base[index2];
        base[index2] = temp;
}

bool SensorModule::isModulePresent(BaseSens* sensModule) {
        if (base == nullptr) return false;
        for (uint8_t i = 0; i < len; i++) {
                if (base[i] == sensModule) {
                        return true;
                }
        }
        return false;
}

bool SensorModule::isModulePresent(uint8_t index) {
        if (base == nullptr || index >= len) return false;
        return true;
}

// ================================================================================================= //
// sensor debugging
SensorDebug::SensorDebug()
    : label("LABEL"), data("DATA"), labelCount(0), dataCount(0), send2Sheet(true), separator(",") {
}

void SensorDebug::init(bool isSendToSheet) {
        this->send2Sheet = isSendToSheet;
        if (!send2Sheet) {
                label = "";
                data = "";
        } else {
                SPI.begin();
                Serial.println("CLEARSHEET");
        }
}

void SensorDebug::addLabel(String _label, bool with_separator) {
        if (with_separator && label != "") label += separator;
        label += _label;
        labelCount++;
}

// gcc ver < C++17 (C++11 / C++14)
template <typename T>
void SensorDebug::addData(T value, bool with_separator) {
        if (with_separator && data != "") data += separator;
        data += String(value);
        dataCount++;
}

void SensorDebug::sendLabel() {
        Serial.println(label);
}

void SensorDebug::sendData(uint32_t __t) {
        if (millis() - send_tm >= __t) {
                if (labelCount != dataCount)
                        Serial.println("Data and Label len didn't match");
                else {
                        Serial.println(data);
                }
                send_tm = millis();
        }
}

void SensorDebug::clearLabel() {
        if (!send2Sheet) {
                labelCount = 0;
                label = "";
        } else {
                labelCount = 0;
                label = "LABEL";
        }
}

void SensorDebug::clearData() {
        if (!send2Sheet) {
                dataCount = 0;
                data = "";
        } else {
                dataCount = 0;
                data = "DATA";
        }
}

void SensorDebug::setLabel(String _label) {
        label = _label;
}

void SensorDebug::setData(String _data) {
        data = _data;
}

void SensorDebug::setSeparator(char _sep[]) {
        separator = _sep;
}

void SensorDebug::setSendDataCallback(void (*sendCallback)(void)) {
        sendCallback();
}

String SensorDebug::getLabel() {
        return label;
}

String SensorDebug::getData() {
        return data;
}

void SensorDebug::reset() {
        label = "LABEL";
        data = "DATA";
        labelCount = 0;
        dataCount = 0;
        Serial.println("CLEARSHEET");
}